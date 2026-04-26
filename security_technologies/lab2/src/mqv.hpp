#ifndef MQV_HPP
#define MQV_HPP

#include "elliptic_curve.hpp"
#include <openssl/bn.h>

// Простые C-style структуры
struct MQVLongTermKeyPair {
    BIGNUM* private_key;
    EllipticCurve::Point public_key;
};

struct MQVEphemeralKeyPair {
    BIGNUM* private_key;
    EllipticCurve::Point public_key;
};

class MQV {
public:
    MQV();
    ~MQV();
    
    MQVLongTermKeyPair* generateLongTermKeyPair();
    MQVEphemeralKeyPair* generateEphemeralKeyPair();
    void freeLongTermKeyPair(MQVLongTermKeyPair* pair);
    void freeEphemeralKeyPair(MQVEphemeralKeyPair* pair);
    
    EllipticCurve::Point* computeSharedSecretC(
        const MQVLongTermKeyPair* c_long_term,
        const MQVEphemeralKeyPair* c_ephemeral,
        const MQVLongTermKeyPair* f_long_term,
        const MQVEphemeralKeyPair* f_ephemeral
    );
    
    EllipticCurve::Point* computeSharedSecretF(
        const MQVLongTermKeyPair* f_long_term,
        const MQVEphemeralKeyPair* f_ephemeral,
        const MQVLongTermKeyPair* c_long_term,
        const MQVEphemeralKeyPair* c_ephemeral
    );
    
    void freePoint(EllipticCurve::Point* p);
    
    void benchmark(int iterations = 100);
    
private:
    EllipticCurve& curve_;
    static const int l = 128;
    
    BIGNUM* random_mod_q();
    BIGNUM* compute_implicit_sig_single(const EllipticCurve::Point& point);
};

#endif // MQV_HPP