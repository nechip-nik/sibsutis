#include "ecdh.hpp"
#include "timer.hpp"
#include <iostream>
#include <iomanip>
#include <openssl/rand.h>

using namespace std;

ECDH::KeyPair::KeyPair() : private_key(nullptr) {
    private_key = BN_new();
    public_key.x = BN_new();
    public_key.y = BN_new();
    public_key.is_infinity = true;
}

ECDH::KeyPair::~KeyPair() {
    if (private_key) {
        BN_free(private_key);
        private_key = nullptr;
    }
    if (public_key.x) {
        BN_free(public_key.x);
        public_key.x = nullptr;
    }
    if (public_key.y) {
        BN_free(public_key.y);
        public_key.y = nullptr;
    }
}

ECDH::KeyPair::KeyPair(KeyPair&& other) noexcept 
    : private_key(other.private_key), public_key(std::move(other.public_key)) {
    other.private_key = nullptr;
    other.public_key.x = nullptr;
    other.public_key.y = nullptr;
}

ECDH::KeyPair& ECDH::KeyPair::operator=(KeyPair&& other) noexcept {
    if (this != &other) {
        if (private_key) BN_free(private_key);
        if (public_key.x) BN_free(public_key.x);
        if (public_key.y) BN_free(public_key.y);
        
        private_key = other.private_key;
        public_key = std::move(other.public_key);
        
        other.private_key = nullptr;
        other.public_key.x = nullptr;
        other.public_key.y = nullptr;
    }
    return *this;
}

ECDH::ECDH() : curve_(EllipticCurve::getInstance()) {}

ECDH::~ECDH() {}

BIGNUM* ECDH::random_mod_q() {
    BIGNUM* result = BN_new();
    BN_rand_range(result, curve_.getQ());
    return result;
}

ECDH::KeyPair ECDH::generateKeyPair() {
    KeyPair pair;
    
    // Generate random private key
    BIGNUM* rand = random_mod_q();
    BN_copy(pair.private_key, rand);
    BN_free(rand);
    
    // Compute public key = private_key * G
    pair.public_key = curve_.scalarMult(pair.private_key, curve_.getG());
    
    return pair;
}

EllipticCurve::Point ECDH::computeSharedSecret(const BIGNUM* private_key, const EllipticCurve::Point& public_key) {
    return curve_.scalarMult(private_key, public_key);
}

void ECDH::benchmark(int iterations) {
    cout << "\n=== ECDH Benchmark (Affine) ===" << endl;
    
    Timer timer;
    uint64_t total_gen_cycles = 0;
    double total_gen_sec = 0;
    
    for (int i = 0; i < iterations; i++) {
        timer.start();
        KeyPair alice = generateKeyPair();
        timer.stop();
        total_gen_cycles += timer.getCycles();
        total_gen_sec += timer.getSeconds();
        // KeyPair will be automatically cleaned up when it goes out of scope
    }
    
    cout << "Key Generation (" << iterations << " iterations):" << endl;
    cout << "  Average cycles: " << total_gen_cycles / iterations << endl;
    cout << "  Average time: " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
    
    // Compute shared secret
    KeyPair alice = generateKeyPair();
    KeyPair bob = generateKeyPair();
    
    total_gen_cycles = 0;
    total_gen_sec = 0;
    
    for (int i = 0; i < iterations; i++) {
        timer.start();
        EllipticCurve::Point secret_alice = computeSharedSecret(alice.private_key, bob.public_key);
        timer.stop();
        total_gen_cycles += timer.getCycles();
        total_gen_sec += timer.getSeconds();
        BN_free(secret_alice.x);
        BN_free(secret_alice.y);
    }
    
    cout << "\nShared Secret Computation (" << iterations << " iterations):" << endl;
    cout << "  Average cycles: " << total_gen_cycles / iterations << endl;
    cout << "  Average time: " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
}