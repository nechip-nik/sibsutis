#include "mqv.hpp"
#include "timer.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

MQV::MQV() : curve_(EllipticCurve::getInstance()) {}

MQV::~MQV() {}

BIGNUM* MQV::random_mod_q() {
    BIGNUM* result = BN_new();
    BN_rand_range(result, curve_.getQ());
    return result;
}

BIGNUM* MQV::compute_implicit_sig_single(const EllipticCurve::Point& point) {
    if (point.is_infinity) {
        BIGNUM* result = BN_new();
        BN_zero(result);
        return result;
    }
    
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* result = BN_new();
    
    BIGNUM* two_pow_l = BN_new();
    BN_set_word(two_pow_l, 1);
    BN_lshift(two_pow_l, two_pow_l, l);
    
    BIGNUM* mask = BN_new();
    BN_set_word(mask, 1);
    BN_lshift(mask, mask, l);
    BN_sub_word(mask, 1);
    
    BIGNUM* x_mod = BN_new();
    BN_mod(x_mod, point.x, mask, ctx);
    
    BN_add(result, two_pow_l, x_mod);
    
    BN_free(two_pow_l);
    BN_free(mask);
    BN_free(x_mod);
    BN_CTX_free(ctx);
    
    return result;
}

MQVLongTermKeyPair* MQV::generateLongTermKeyPair() {
    MQVLongTermKeyPair* pair = new MQVLongTermKeyPair();
    
    // Create private key
    pair->private_key = random_mod_q();
    
    // Compute public key
    EllipticCurve::Point pub = curve_.scalarMult(pair->private_key, curve_.getG());
    
    // Create new points for public key
    pair->public_key.x = BN_new();
    pair->public_key.y = BN_new();
    BN_copy(pair->public_key.x, pub.x);
    BN_copy(pair->public_key.y, pub.y);
    pair->public_key.is_infinity = pub.is_infinity;
    
    // Clean up temporary point
    BN_free(pub.x);
    BN_free(pub.y);
    
    return pair;
}

MQVEphemeralKeyPair* MQV::generateEphemeralKeyPair() {
    MQVEphemeralKeyPair* pair = new MQVEphemeralKeyPair();
    
    // Create private key
    pair->private_key = random_mod_q();
    
    // Compute public key
    EllipticCurve::Point pub = curve_.scalarMult(pair->private_key, curve_.getG());
    
    // Create new points for public key
    pair->public_key.x = BN_new();
    pair->public_key.y = BN_new();
    BN_copy(pair->public_key.x, pub.x);
    BN_copy(pair->public_key.y, pub.y);
    pair->public_key.is_infinity = pub.is_infinity;
    
    // Clean up temporary point
    BN_free(pub.x);
    BN_free(pub.y);
    
    return pair;
}

void MQV::freeLongTermKeyPair(MQVLongTermKeyPair* pair) {
    if (pair) {
        if (pair->private_key) BN_free(pair->private_key);
        if (pair->public_key.x) BN_free(pair->public_key.x);
        if (pair->public_key.y) BN_free(pair->public_key.y);
        delete pair;
    }
}

void MQV::freeEphemeralKeyPair(MQVEphemeralKeyPair* pair) {
    if (pair) {
        if (pair->private_key) BN_free(pair->private_key);
        if (pair->public_key.x) BN_free(pair->public_key.x);
        if (pair->public_key.y) BN_free(pair->public_key.y);
        delete pair;
    }
}

void MQV::freePoint(EllipticCurve::Point* p) {
    if (p) {
        if (p->x) BN_free(p->x);
        if (p->y) BN_free(p->y);
        delete p;
    }
}

EllipticCurve::Point* MQV::computeSharedSecretC(
    const MQVLongTermKeyPair* c_long_term,
    const MQVEphemeralKeyPair* c_ephemeral,
    const MQVLongTermKeyPair* f_long_term,
    const MQVEphemeralKeyPair* f_ephemeral
) {
    if (!c_long_term || !c_ephemeral || !f_long_term || !f_ephemeral) {
        return nullptr;
    }
    
    BIGNUM* d = compute_implicit_sig_single(c_ephemeral->public_key);
    BIGNUM* e = compute_implicit_sig_single(f_ephemeral->public_key);
    
    BN_CTX* ctx = BN_CTX_new();
    
    // Compute u + d*c mod q
    BIGNUM* dc = BN_new();
    BN_mod_mul(dc, d, c_long_term->private_key, curve_.getQ(), ctx);
    BIGNUM* u_plus_dc = BN_new();
    BN_mod_add(u_plus_dc, c_ephemeral->private_key, dc, curve_.getQ(), ctx);
    
    // Compute V + [e]F
    EllipticCurve::Point eF = curve_.scalarMult(e, f_long_term->public_key);
    EllipticCurve::Point V_plus_eF = curve_.pointAdd(f_ephemeral->public_key, eF);
    
    // Compute S = [u + d*c](V + [e]F)
    EllipticCurve::Point S = curve_.scalarMult(u_plus_dc, V_plus_eF);
    
    // Create result point
    EllipticCurve::Point* result = new EllipticCurve::Point();
    result->x = BN_new();
    result->y = BN_new();
    BN_copy(result->x, S.x);
    BN_copy(result->y, S.y);
    result->is_infinity = S.is_infinity;
    
    // Clean up
    BN_free(d);
    BN_free(e);
    BN_free(dc);
    BN_free(u_plus_dc);
    BN_free(eF.x);
    BN_free(eF.y);
    BN_free(V_plus_eF.x);
    BN_free(V_plus_eF.y);
    BN_free(S.x);
    BN_free(S.y);
    BN_CTX_free(ctx);
    
    return result;
}

EllipticCurve::Point* MQV::computeSharedSecretF(
    const MQVLongTermKeyPair* f_long_term,
    const MQVEphemeralKeyPair* f_ephemeral,
    const MQVLongTermKeyPair* c_long_term,
    const MQVEphemeralKeyPair* c_ephemeral
) {
    if (!f_long_term || !f_ephemeral || !c_long_term || !c_ephemeral) {
        return nullptr;
    }
    
    BIGNUM* d = compute_implicit_sig_single(c_ephemeral->public_key);
    BIGNUM* e = compute_implicit_sig_single(f_ephemeral->public_key);
    
    BN_CTX* ctx = BN_CTX_new();
    
    // Compute v + e*f mod q
    BIGNUM* ef = BN_new();
    BN_mod_mul(ef, e, f_long_term->private_key, curve_.getQ(), ctx);
    BIGNUM* v_plus_ef = BN_new();
    BN_mod_add(v_plus_ef, f_ephemeral->private_key, ef, curve_.getQ(), ctx);
    
    // Compute U + [d]C
    EllipticCurve::Point dC = curve_.scalarMult(d, c_long_term->public_key);
    EllipticCurve::Point U_plus_dC = curve_.pointAdd(c_ephemeral->public_key, dC);
    
    // Compute S = [v + e*f](U + [d]C)
    EllipticCurve::Point S = curve_.scalarMult(v_plus_ef, U_plus_dC);
    
    // Create result point
    EllipticCurve::Point* result = new EllipticCurve::Point();
    result->x = BN_new();
    result->y = BN_new();
    BN_copy(result->x, S.x);
    BN_copy(result->y, S.y);
    result->is_infinity = S.is_infinity;
    
    // Clean up
    BN_free(d);
    BN_free(e);
    BN_free(ef);
    BN_free(v_plus_ef);
    BN_free(dC.x);
    BN_free(dC.y);
    BN_free(U_plus_dC.x);
    BN_free(U_plus_dC.y);
    BN_free(S.x);
    BN_free(S.y);
    BN_CTX_free(ctx);
    
    return result;
}

void MQV::benchmark(int iterations) {
    cout << "\n=== MQV Benchmark (Affine) ===" << endl;
    
    Timer timer;
    uint64_t total_lt_gen_cycles = 0;
    double total_lt_gen_sec = 0;
    
    for (int i = 0; i < iterations; i++) {
        timer.start();
        MQVLongTermKeyPair* c_lt = generateLongTermKeyPair();
        MQVLongTermKeyPair* f_lt = generateLongTermKeyPair();
        timer.stop();
        total_lt_gen_cycles += timer.getCycles();
        total_lt_gen_sec += timer.getSeconds();
        
        freeLongTermKeyPair(c_lt);
        freeLongTermKeyPair(f_lt);
    }
    
    cout << "Long-term Key Generation (2 pairs, " << iterations << " iterations):" << endl;
    cout << "  Average cycles: " << total_lt_gen_cycles / iterations << endl;
    cout << "  Average time: " << (total_lt_gen_sec / iterations) * 1e6 << " μs" << endl;
    
    MQVLongTermKeyPair* c_lt = generateLongTermKeyPair();
    MQVLongTermKeyPair* f_lt = generateLongTermKeyPair();
    
    total_lt_gen_cycles = 0;
    total_lt_gen_sec = 0;
    
    for (int i = 0; i < iterations; i++) {
        timer.start();
        MQVEphemeralKeyPair* c_eph = generateEphemeralKeyPair();
        MQVEphemeralKeyPair* f_eph = generateEphemeralKeyPair();
        
        EllipticCurve::Point* S_c = computeSharedSecretC(c_lt, c_eph, f_lt, f_eph);
        EllipticCurve::Point* S_f = computeSharedSecretF(f_lt, f_eph, c_lt, c_eph);
        timer.stop();
        
        total_lt_gen_cycles += timer.getCycles();
        total_lt_gen_sec += timer.getSeconds();
        
        freeEphemeralKeyPair(c_eph);
        freeEphemeralKeyPair(f_eph);
        freePoint(S_c);
        freePoint(S_f);
    }
    
    cout << "\nFull MQV Protocol (ephemeral keys + shared secret, " << iterations << " iterations):" << endl;
    cout << "  Average cycles: " << total_lt_gen_cycles / iterations << endl;
    cout << "  Average time: " << (total_lt_gen_sec / iterations) * 1e6 << " μs" << endl;
    
    freeLongTermKeyPair(c_lt);
    freeLongTermKeyPair(f_lt);
}