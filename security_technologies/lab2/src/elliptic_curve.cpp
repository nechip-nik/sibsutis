#include "elliptic_curve.hpp"
#include <openssl/obj_mac.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>

using namespace std;

static BN_CTX* getBNCtx() {
    thread_local BN_CTX* ctx = BN_CTX_new();
    return ctx;
}

EllipticCurve::EllipticCurve() {
    BN_CTX* ctx = getBNCtx();
    
    p_ = BN_new();
    a_ = BN_new();
    b_ = BN_new();
    q_ = BN_new();
    
    // P-256 prime: 2^256 - 2^224 + 2^192 + 2^96 - 1
    BN_hex2bn(&p_, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");
    // a = -3 mod p
    BN_hex2bn(&a_, "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC");
    // b
    BN_hex2bn(&b_, "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");
    // q (order of G)
    BN_hex2bn(&q_, "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551");
    
    g_.x = BN_new();
    g_.y = BN_new();
    BN_hex2bn(&g_.x, "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296");
    BN_hex2bn(&g_.y, "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECCBBA6406837BF51F5");
    g_.is_infinity = false;
}

EllipticCurve::~EllipticCurve() {
    BN_free(p_);
    BN_free(a_);
    BN_free(b_);
    BN_free(q_);
    BN_free(g_.x);
    BN_free(g_.y);
}

EllipticCurve& EllipticCurve::getInstance() {
    static EllipticCurve instance;
    return instance;
}

EllipticCurve::Point::Point() : x(BN_new()), y(BN_new()), is_infinity(true) {}

EllipticCurve::Point::Point(const Point& other) : x(BN_new()), y(BN_new()), is_infinity(other.is_infinity) {
    BN_copy(x, other.x);
    BN_copy(y, other.y);
}

EllipticCurve::Point::~Point() {
    if (x) BN_free(x);
    if (y) BN_free(y);
}

EllipticCurve::Point& EllipticCurve::Point::operator=(const Point& other) {
    if (this != &other) {
        BN_copy(x, other.x);
        BN_copy(y, other.y);
        is_infinity = other.is_infinity;
    }
    return *this;
}

string EllipticCurve::Point::toString() const {
    if (is_infinity) return "INFINITY";
    char* x_str = BN_bn2hex(x);
    char* y_str = BN_bn2hex(y);
    string result = string("(") + x_str + ", " + y_str + ")";
    OPENSSL_free(x_str);
    OPENSSL_free(y_str);
    return result;
}

EllipticCurve::ProjectivePoint::ProjectivePoint() 
    : x(BN_new()), y(BN_new()), z(BN_new()), is_infinity(true) {
    BN_one(z);
}

EllipticCurve::ProjectivePoint::ProjectivePoint(const ProjectivePoint& other)
    : x(BN_new()), y(BN_new()), z(BN_new()), is_infinity(other.is_infinity) {
    BN_copy(x, other.x);
    BN_copy(y, other.y);
    BN_copy(z, other.z);
}

EllipticCurve::ProjectivePoint::~ProjectivePoint() {
    if (x) BN_free(x);
    if (y) BN_free(y);
    if (z) BN_free(z);
}

EllipticCurve::ProjectivePoint& EllipticCurve::ProjectivePoint::operator=(const ProjectivePoint& other) {
    if (this != &other) {
        BN_copy(x, other.x);
        BN_copy(y, other.y);
        BN_copy(z, other.z);
        is_infinity = other.is_infinity;
    }
    return *this;
}

BIGNUM* EllipticCurve::mod(const BIGNUM* a) const {
    BN_CTX* ctx = getBNCtx();
    BIGNUM* result = BN_new();
    BN_nnmod(result, a, p_, ctx);
    return result;
}

BIGNUM* EllipticCurve::modAdd(const BIGNUM* a, const BIGNUM* b) const {
    BN_CTX* ctx = getBNCtx();
    BIGNUM* result = BN_new();
    BN_mod_add(result, a, b, p_, ctx);
    return result;
}

BIGNUM* EllipticCurve::modSub(const BIGNUM* a, const BIGNUM* b) const {
    BN_CTX* ctx = getBNCtx();
    BIGNUM* result = BN_new();
    BN_mod_sub(result, a, b, p_, ctx);
    return result;
}

BIGNUM* EllipticCurve::modMul(const BIGNUM* a, const BIGNUM* b) const {
    BN_CTX* ctx = getBNCtx();
    BIGNUM* result = BN_new();
    BN_mod_mul(result, a, b, p_, ctx);
    return result;
}

BIGNUM* EllipticCurve::modInv(const BIGNUM* a) const {
    BN_CTX* ctx = getBNCtx();
    BIGNUM* result = BN_new();
    BN_mod_inverse(result, a, p_, ctx);
    return result;
}

EllipticCurve::Point EllipticCurve::pointAdd(const Point& P, const Point& Q) const {
    Point R;
    
    if (P.is_infinity) {
        R = Q;
        return R;
    }
    if (Q.is_infinity) {
        R = P;
        return R;
    }
    
    BN_CTX* ctx = getBNCtx();
    
    if (BN_cmp(P.x, Q.x) == 0) {
        if (BN_cmp(P.y, Q.y) == 0) {
            return pointDouble(P);
        } else {
            R.is_infinity = true;
            return R;
        }
    }
    
    BIGNUM* dy = modSub(Q.y, P.y);
    BIGNUM* dx = modSub(Q.x, P.x);
    BIGNUM* dx_inv = modInv(dx);
    BIGNUM* lambda = modMul(dy, dx_inv);
    
    BIGNUM* lambda_sq = modMul(lambda, lambda);
    BIGNUM* x3 = modSub(lambda_sq, P.x);
    x3 = modSub(x3, Q.x);
    
    BIGNUM* x1_minus_x3 = modSub(P.x, x3);
    BIGNUM* y3 = modMul(lambda, x1_minus_x3);
    y3 = modSub(y3, P.y);
    
    BN_copy(R.x, x3);
    BN_copy(R.y, y3);
    R.is_infinity = false;
    
    BN_free(dy);
    BN_free(dx);
    BN_free(dx_inv);
    BN_free(lambda);
    BN_free(lambda_sq);
    BN_free(x1_minus_x3);
    BN_free(x3);
    BN_free(y3);
    
    return R;
}

EllipticCurve::Point EllipticCurve::pointDouble(const Point& P) const {
    Point R;
    
    if (P.is_infinity) {
        R.is_infinity = true;
        return R;
    }
    
    BN_CTX* ctx = getBNCtx();
    
    BIGNUM* x_sq = modMul(P.x, P.x);
    BIGNUM* three_x_sq = BN_new();
    BN_copy(three_x_sq, x_sq);
    BN_add_word(three_x_sq, 2);
    BN_mod_mul(three_x_sq, three_x_sq, x_sq, p_, ctx);
    
    BIGNUM* numerator = modAdd(three_x_sq, a_);
    BIGNUM* two_y = modAdd(P.y, P.y);
    BIGNUM* denominator_inv = modInv(two_y);
    BIGNUM* lambda = modMul(numerator, denominator_inv);
    
    BIGNUM* lambda_sq = modMul(lambda, lambda);
    BIGNUM* two_x = modAdd(P.x, P.x);
    BIGNUM* x3 = modSub(lambda_sq, two_x);
    
    BIGNUM* x1_minus_x3 = modSub(P.x, x3);
    BIGNUM* y3 = modMul(lambda, x1_minus_x3);
    y3 = modSub(y3, P.y);
    
    BN_copy(R.x, x3);
    BN_copy(R.y, y3);
    R.is_infinity = false;
    
    BN_free(x_sq);
    BN_free(three_x_sq);
    BN_free(numerator);
    BN_free(two_y);
    BN_free(denominator_inv);
    BN_free(lambda);
    BN_free(lambda_sq);
    BN_free(two_x);
    BN_free(x1_minus_x3);
    BN_free(x3);
    BN_free(y3);
    
    return R;
}

EllipticCurve::Point EllipticCurve::scalarMult(const BIGNUM* k, const Point& P) const {
    Point result;
    result.is_infinity = true;
    
    Point base = P;
    
    int bits = BN_num_bits(k);
    for (int i = 0; i < bits; i++) {
        if (BN_is_bit_set(k, i)) {
            result = pointAdd(result, base);
        }
        base = pointDouble(base);
    }
    
    return result;
}

EllipticCurve::ProjectivePoint EllipticCurve::toProjective(const Point& P) const {
    ProjectivePoint R;
    if (P.is_infinity) {
        R.is_infinity = true;
        return R;
    }
    
    BN_copy(R.x, P.x);
    BN_copy(R.y, P.y);
    BN_one(R.z);
    R.is_infinity = false;
    return R;
}

EllipticCurve::Point EllipticCurve::toAffine(const ProjectivePoint& P) const {
    Point R;
    if (P.is_infinity) {
        R.is_infinity = true;
        return R;
    }
    
    BN_CTX* ctx = getBNCtx();
    
    BIGNUM* z_sq = modMul(P.z, P.z);
    BIGNUM* z_cu = modMul(z_sq, P.z);
    BIGNUM* z_sq_inv = modInv(z_sq);
    BIGNUM* z_cu_inv = modInv(z_cu);
    
    BN_mod_mul(R.x, P.x, z_sq_inv, p_, ctx);
    BN_mod_mul(R.y, P.y, z_cu_inv, p_, ctx);
    R.is_infinity = false;
    
    BN_free(z_sq);
    BN_free(z_cu);
    BN_free(z_sq_inv);
    BN_free(z_cu_inv);
    
    return R;
}

EllipticCurve::ProjectivePoint EllipticCurve::pointAddProjective(const ProjectivePoint& P, const ProjectivePoint& Q) const {
    Point P_aff = toAffine(P);
    Point Q_aff = toAffine(Q);
    Point R_aff = pointAdd(P_aff, Q_aff);
    ProjectivePoint R = toProjective(R_aff);
    
    BN_free(P_aff.x);
    BN_free(P_aff.y);
    BN_free(Q_aff.x);
    BN_free(Q_aff.y);
    BN_free(R_aff.x);
    BN_free(R_aff.y);
    
    return R;
}

EllipticCurve::ProjectivePoint EllipticCurve::pointDoubleProjective(const ProjectivePoint& P) const {
    Point P_aff = toAffine(P);
    Point R_aff = pointDouble(P_aff);
    ProjectivePoint R = toProjective(R_aff);
    
    BN_free(P_aff.x);
    BN_free(P_aff.y);
    BN_free(R_aff.x);
    BN_free(R_aff.y);
    
    return R;
}

EllipticCurve::ProjectivePoint EllipticCurve::scalarMultProjective(const BIGNUM* k, const Point& P) const {
    ProjectivePoint result;
    result.is_infinity = true;
    
    ProjectivePoint base = toProjective(P);
    
    int bits = BN_num_bits(k);
    for (int i = 0; i < bits; i++) {
        if (BN_is_bit_set(k, i)) {
            result = pointAddProjective(result, base);
        }
        base = pointDoubleProjective(base);
    }
    
    return result;
}

bool EllipticCurve::isOnCurve(const Point& P) const {
    if (P.is_infinity) return true;
    
    BN_CTX* ctx = getBNCtx();
    
    BIGNUM* x_sq = modMul(P.x, P.x);
    BIGNUM* x_cu = modMul(x_sq, P.x);
    BIGNUM* ax = modMul(a_, P.x);
    BIGNUM* right = modAdd(x_cu, ax);
    right = modAdd(right, b_);
    
    BIGNUM* left = modMul(P.y, P.y);
    
    int cmp = BN_cmp(left, right);
    
    BN_free(x_sq);
    BN_free(x_cu);
    BN_free(ax);
    BN_free(right);
    BN_free(left);
    
    return cmp == 0;
}

bool EllipticCurve::isOnCurve(const ProjectivePoint& P) const {
    Point affine = toAffine(P);
    bool result = isOnCurve(affine);
    BN_free(affine.x);
    BN_free(affine.y);
    return result;
}
// Move constructor for Point
EllipticCurve::Point::Point(Point&& other) noexcept
    : x(other.x), y(other.y), is_infinity(other.is_infinity) {
    other.x = nullptr;
    other.y = nullptr;
}

// Move assignment for Point
EllipticCurve::Point& EllipticCurve::Point::operator=(Point&& other) noexcept {
    if (this != &other) {
        if (x) BN_free(x);
        if (y) BN_free(y);
        
        x = other.x;
        y = other.y;
        is_infinity = other.is_infinity;
        
        other.x = nullptr;
        other.y = nullptr;
    }
    return *this;
}

// Move constructor for ProjectivePoint
EllipticCurve::ProjectivePoint::ProjectivePoint(ProjectivePoint&& other) noexcept
    : x(other.x), y(other.y), z(other.z), is_infinity(other.is_infinity) {
    other.x = nullptr;
    other.y = nullptr;
    other.z = nullptr;
}

// Move assignment for ProjectivePoint
EllipticCurve::ProjectivePoint& EllipticCurve::ProjectivePoint::operator=(ProjectivePoint&& other) noexcept {
    if (this != &other) {
        if (x) BN_free(x);
        if (y) BN_free(y);
        if (z) BN_free(z);
        
        x = other.x;
        y = other.y;
        z = other.z;
        is_infinity = other.is_infinity;
        
        other.x = nullptr;
        other.y = nullptr;
        other.z = nullptr;
    }
    return *this;
}