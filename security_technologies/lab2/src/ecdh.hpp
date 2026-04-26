#ifndef ECDH_HPP
#define ECDH_HPP

#include "elliptic_curve.hpp"
#include <openssl/bn.h>
#include <memory>

class ECDH {
public:
    struct KeyPair {
        BIGNUM* private_key;
        EllipticCurve::Point public_key;
        
        KeyPair();
        ~KeyPair();
        // Запрещаем копирование
        KeyPair(const KeyPair&) = delete;
        KeyPair& operator=(const KeyPair&) = delete;
        // Разрешаем перемещение
        KeyPair(KeyPair&& other) noexcept;
        KeyPair& operator=(KeyPair&& other) noexcept;
    };
    
    ECDH();
    ~ECDH();
    
    KeyPair generateKeyPair();
    EllipticCurve::Point computeSharedSecret(const BIGNUM* private_key, const EllipticCurve::Point& public_key);
    
    // For benchmarking
    void benchmark(int iterations = 1000);
    
private:
    EllipticCurve& curve_;
    BIGNUM* random_mod_q();
};

#endif // ECDH_HPP