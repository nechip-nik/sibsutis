#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <openssl/rand.h>

using namespace std;

// Timer class for benchmarking (без __rdtsc для совместимости)
class Timer {
private:
    chrono::high_resolution_clock::time_point start_time;
    chrono::high_resolution_clock::time_point end_time;
    
public:
    void start() {
        start_time = chrono::high_resolution_clock::now();
    }
    
    void stop() {
        end_time = chrono::high_resolution_clock::now();
    }
    
    double getSeconds() const {
        return chrono::duration<double>(end_time - start_time).count();
    }
    
    uint64_t getCycles() const {
        // Возвращаем наносекунды вместо циклов для совместимости
        return (uint64_t)(getSeconds() * 1e9);
    }
};

// Функция для преобразования BIGNUM в hex строку
string bnToHex(BIGNUM* bn) {
    char* hex = BN_bn2hex(bn);
    string result(hex);
    OPENSSL_free(hex);
    return result;
}

// ECDH implementation
class ECDH {
private:
    EC_KEY* key;
    const EC_GROUP* group;
    
public:
    ECDH() {
        key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        EC_KEY_generate_key(key);
        group = EC_KEY_get0_group(key);
    }
    
    ~ECDH() {
        if (key) EC_KEY_free(key);
    }
    
    const EC_POINT* getPublicKey() const {
        return EC_KEY_get0_public_key(key);
    }
    
    const BIGNUM* getPrivateKey() const {
        return EC_KEY_get0_private_key(key);
    }
    
    void computeSharedSecret(const EC_POINT* peer_public, vector<uint8_t>& secret) {
        EC_POINT* shared = EC_POINT_new(group);
        BN_CTX* ctx = BN_CTX_new();
        
        // Compute shared point = private * peer_public
        EC_POINT_mul(group, shared, NULL, peer_public, EC_KEY_get0_private_key(key), ctx);
        
        // Get x-coordinate as shared secret
        BIGNUM* x = BN_new();
        EC_POINT_get_affine_coordinates(group, shared, x, NULL, ctx);
        
        // Convert to bytes
        size_t len = BN_num_bytes(x);
        secret.resize(len);
        BN_bn2bin(x, secret.data());
        
        BN_free(x);
        EC_POINT_free(shared);
        BN_CTX_free(ctx);
    }
    
    string getPublicKeyHex() const {
        const EC_POINT* pub = EC_KEY_get0_public_key(key);
        BN_CTX* ctx = BN_CTX_new();
        BIGNUM* x = BN_new();
        BIGNUM* y = BN_new();
        
        EC_POINT_get_affine_coordinates(group, pub, x, y, ctx);
        
        string result = bnToHex(x) + bnToHex(y);
        
        BN_free(x);
        BN_free(y);
        BN_CTX_free(ctx);
        
        return result;
    }
    
    void benchmark(int iterations = 1000) {
        cout << "\n=== ECDH Benchmark ===" << endl;
        
        Timer timer;
        uint64_t total_gen = 0;
        double total_gen_sec = 0;
        
        for (int i = 0; i < iterations; i++) {
            timer.start();
            EC_KEY* tmp_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
            EC_KEY_generate_key(tmp_key);
            timer.stop();
            total_gen += timer.getCycles();
            total_gen_sec += timer.getSeconds();
            EC_KEY_free(tmp_key);
        }
        
        cout << "Key Generation (avg cycles): " << total_gen / iterations << endl;
        cout << "Key Generation (avg time): " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
        
        // Compute shared secret benchmark
        EC_KEY* alice = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        EC_KEY* bob = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        EC_KEY_generate_key(alice);
        EC_KEY_generate_key(bob);
        
        const EC_GROUP* grp = EC_KEY_get0_group(alice);
        const EC_POINT* bob_pub = EC_KEY_get0_public_key(bob);
        
        total_gen = 0;
        total_gen_sec = 0;
        
        for (int i = 0; i < iterations; i++) {
            timer.start();
            EC_POINT* shared = EC_POINT_new(grp);
            BN_CTX* ctx = BN_CTX_new();
            EC_POINT_mul(grp, shared, NULL, bob_pub, EC_KEY_get0_private_key(alice), ctx);
            BN_CTX_free(ctx);
            EC_POINT_free(shared);
            timer.stop();
            total_gen += timer.getCycles();
            total_gen_sec += timer.getSeconds();
        }
        
        cout << "Shared Secret (avg cycles): " << total_gen / iterations << endl;
        cout << "Shared Secret (avg time): " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
        
        EC_KEY_free(alice);
        EC_KEY_free(bob);
    }
};

// MQV implementation
class MQV {
private:
    const EC_GROUP* group;
    static const int l = 128;
    
    BIGNUM* computeImplicitSig(const EC_POINT* point) {
        BN_CTX* ctx = BN_CTX_new();
        BIGNUM* x = BN_new();
        EC_POINT_get_affine_coordinates(group, point, x, NULL, ctx);
        
        BIGNUM* two_pow_l = BN_new();
        BN_set_word(two_pow_l, 1);
        BN_lshift(two_pow_l, two_pow_l, l);
        
        BIGNUM* mask = BN_new();
        BN_set_word(mask, 1);
        BN_lshift(mask, mask, l);
        BN_sub_word(mask, 1);
        
        BIGNUM* x_mod = BN_new();
        BN_mod(x_mod, x, mask, ctx);
        
        BIGNUM* result = BN_new();
        BN_add(result, two_pow_l, x_mod);
        
        BN_free(x);
        BN_free(two_pow_l);
        BN_free(mask);
        BN_free(x_mod);
        BN_CTX_free(ctx);
        
        return result;
    }
    
public:
    MQV() {
        group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    }
    
    ~MQV() {
        if (group) EC_GROUP_free((EC_GROUP*)group);
    }
    
    EC_KEY* generateKeyPair() {
        EC_KEY* key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        EC_KEY_generate_key(key);
        return key;
    }
    
    void computeSharedSecretC(
        EC_KEY* c_long, EC_KEY* c_eph,
        EC_KEY* f_long, EC_KEY* f_eph,
        vector<uint8_t>& secret
    ) {
        BIGNUM* d = computeImplicitSig(EC_KEY_get0_public_key(c_eph));
        BIGNUM* e = computeImplicitSig(EC_KEY_get0_public_key(f_eph));
        
        BN_CTX* ctx = BN_CTX_new();
        const BIGNUM* order = EC_GROUP_get0_order(group);
        
        // Compute u + d*c mod q
        const BIGNUM* u = EC_KEY_get0_private_key(c_eph);
        const BIGNUM* c = EC_KEY_get0_private_key(c_long);
        
        BIGNUM* dc = BN_new();
        BN_mod_mul(dc, d, c, order, ctx);
        
        BIGNUM* u_plus_dc = BN_new();
        BN_mod_add(u_plus_dc, u, dc, order, ctx);
        
        // Compute V + [e]F
        EC_POINT* eF = EC_POINT_new(group);
        EC_POINT_mul(group, eF, NULL, EC_KEY_get0_public_key(f_long), e, ctx);
        
        EC_POINT* V_plus_eF = EC_POINT_new(group);
        EC_POINT_add(group, V_plus_eF, EC_KEY_get0_public_key(f_eph), eF, ctx);
        
        // Compute S = [u + d*c](V + [e]F)
        EC_POINT* S = EC_POINT_new(group);
        EC_POINT_mul(group, S, NULL, V_plus_eF, u_plus_dc, ctx);
        
        // Get x-coordinate as secret
        BIGNUM* x = BN_new();
        EC_POINT_get_affine_coordinates(group, S, x, NULL, ctx);
        
        size_t len = BN_num_bytes(x);
        secret.resize(len);
        BN_bn2bin(x, secret.data());
        
        BN_free(d);
        BN_free(e);
        BN_free(dc);
        BN_free(u_plus_dc);
        BN_free(x);
        EC_POINT_free(eF);
        EC_POINT_free(V_plus_eF);
        EC_POINT_free(S);
        BN_CTX_free(ctx);
    }
    
    void computeSharedSecretF(
        EC_KEY* f_long, EC_KEY* f_eph,
        EC_KEY* c_long, EC_KEY* c_eph,
        vector<uint8_t>& secret
    ) {
        BIGNUM* d = computeImplicitSig(EC_KEY_get0_public_key(c_eph));
        BIGNUM* e = computeImplicitSig(EC_KEY_get0_public_key(f_eph));
        
        BN_CTX* ctx = BN_CTX_new();
        const BIGNUM* order = EC_GROUP_get0_order(group);
        
        // Compute v + e*f mod q
        const BIGNUM* v = EC_KEY_get0_private_key(f_eph);
        const BIGNUM* f = EC_KEY_get0_private_key(f_long);
        
        BIGNUM* ef = BN_new();
        BN_mod_mul(ef, e, f, order, ctx);
        
        BIGNUM* v_plus_ef = BN_new();
        BN_mod_add(v_plus_ef, v, ef, order, ctx);
        
        // Compute U + [d]C
        EC_POINT* dC = EC_POINT_new(group);
        EC_POINT_mul(group, dC, NULL, EC_KEY_get0_public_key(c_long), d, ctx);
        
        EC_POINT* U_plus_dC = EC_POINT_new(group);
        EC_POINT_add(group, U_plus_dC, EC_KEY_get0_public_key(c_eph), dC, ctx);
        
        // Compute S = [v + e*f](U + [d]C)
        EC_POINT* S = EC_POINT_new(group);
        EC_POINT_mul(group, S, NULL, U_plus_dC, v_plus_ef, ctx);
        
        // Get x-coordinate as secret
        BIGNUM* x = BN_new();
        EC_POINT_get_affine_coordinates(group, S, x, NULL, ctx);
        
        size_t len = BN_num_bytes(x);
        secret.resize(len);
        BN_bn2bin(x, secret.data());
        
        BN_free(d);
        BN_free(e);
        BN_free(ef);
        BN_free(v_plus_ef);
        BN_free(x);
        EC_POINT_free(dC);
        EC_POINT_free(U_plus_dC);
        EC_POINT_free(S);
        BN_CTX_free(ctx);
    }
    
    void benchmark(int iterations = 100) {
        cout << "\n=== MQV Benchmark ===" << endl;
        
        Timer timer;
        uint64_t total_gen = 0;
        double total_gen_sec = 0;
        
        for (int i = 0; i < iterations; i++) {
            timer.start();
            EC_KEY* c_long = generateKeyPair();
            EC_KEY* f_long = generateKeyPair();
            timer.stop();
            total_gen += timer.getCycles();
            total_gen_sec += timer.getSeconds();
            EC_KEY_free(c_long);
            EC_KEY_free(f_long);
        }
        
        cout << "Long-term Key Generation (avg cycles): " << total_gen / iterations << endl;
        cout << "Long-term Key Generation (avg time): " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
        
        EC_KEY* c_long = generateKeyPair();
        EC_KEY* f_long = generateKeyPair();
        
        total_gen = 0;
        total_gen_sec = 0;
        
        for (int i = 0; i < iterations; i++) {
            timer.start();
            EC_KEY* c_eph = generateKeyPair();
            EC_KEY* f_eph = generateKeyPair();
            
            vector<uint8_t> secret_c, secret_f;
            computeSharedSecretC(c_long, c_eph, f_long, f_eph, secret_c);
            computeSharedSecretF(f_long, f_eph, c_long, c_eph, secret_f);
            timer.stop();
            
            total_gen += timer.getCycles();
            total_gen_sec += timer.getSeconds();
            
            EC_KEY_free(c_eph);
            EC_KEY_free(f_eph);
        }
        
        cout << "Full MQV Protocol (avg cycles): " << total_gen / iterations << endl;
        cout << "Full MQV Protocol (avg time): " << (total_gen_sec / iterations) * 1e6 << " μs" << endl;
        
        EC_KEY_free(c_long);
        EC_KEY_free(f_long);
    }
};

string hexStr(const vector<uint8_t>& data) {
    stringstream ss;
    ss << hex << setfill('0');
    for (uint8_t byte : data) {
        ss << setw(2) << (int)byte;
    }
    return ss.str();
}

int main() {
    cout << "========================================" << endl;
    cout << "ECDH and MQV Protocol Implementation" << endl;
    cout << "Elliptic Curve: P-256 (secp256r1)" << endl;
    cout << "========================================" << endl;
    
    try {
        // Test ECDH
        cout << "\n--- Testing ECDH ---" << endl;
        
        ECDH alice, bob;
        
        cout << "Alice's public key: " << alice.getPublicKeyHex() << endl;
        cout << "Bob's public key: " << bob.getPublicKeyHex() << endl;
        
        vector<uint8_t> secret_alice, secret_bob;
        alice.computeSharedSecret(bob.getPublicKey(), secret_alice);
        bob.computeSharedSecret(alice.getPublicKey(), secret_bob);
        
        cout << "Alice's shared secret: " << hexStr(secret_alice) << endl;
        cout << "Bob's shared secret: " << hexStr(secret_bob) << endl;
        
        if (secret_alice == secret_bob) {
            cout << "✓ Shared secrets match!" << endl;
        } else {
            cout << "✗ Shared secrets do not match!" << endl;
        }
        
        cout << "\nECDH test completed successfully!" << endl;
        
        // Test MQV
        cout << "\n--- Testing MQV ---" << endl;
        
        MQV mqv;
        EC_KEY* c_long = mqv.generateKeyPair();
        EC_KEY* f_long = mqv.generateKeyPair();
        EC_KEY* c_eph = mqv.generateKeyPair();
        EC_KEY* f_eph = mqv.generateKeyPair();
        
        vector<uint8_t> secret_c, secret_f;
        mqv.computeSharedSecretC(c_long, c_eph, f_long, f_eph, secret_c);
        mqv.computeSharedSecretF(f_long, f_eph, c_long, c_eph, secret_f);
        
        cout << "C's shared secret: " << hexStr(secret_c) << endl;
        cout << "F's shared secret: " << hexStr(secret_f) << endl;
        
        if (secret_c == secret_f) {
            cout << "✓ Shared secrets match!" << endl;
        } else {
            cout << "✗ Shared secrets do not match!" << endl;
        }
        
        EC_KEY_free(c_long);
        EC_KEY_free(f_long);
        EC_KEY_free(c_eph);
        EC_KEY_free(f_eph);
        
        cout << "\nMQV test completed successfully!" << endl;
        
        // Run benchmarks
        cout << "\n========================================" << endl;
        cout << "Running Benchmarks" << endl;
        cout << "========================================" << endl;
        
        ECDH ecdh_bench;
        ecdh_bench.benchmark(1000);
        
        MQV mqv_bench;
        mqv_bench.benchmark(100);
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nAll tests completed successfully!" << endl;
    return 0;
}