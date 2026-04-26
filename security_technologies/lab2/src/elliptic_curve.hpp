#ifndef ELLIPTIC_CURVE_HPP
#define ELLIPTIC_CURVE_HPP

#include <openssl/bn.h>
#include <memory>
#include <string>

// Curve P-256 parameters
class EllipticCurve {
public:
    // Singleton pattern
    static EllipticCurve& getInstance();
    
    // Point representation
    struct Point {
        BIGNUM* x;
        BIGNUM* y;
        bool is_infinity;
        
        Point();
        Point(const Point& other);
        ~Point();
        Point& operator=(const Point& other);
        Point(Point&& other) noexcept;
        Point& operator=(Point&& other) noexcept;
        std::string toString() const;
    };
    
    // Projective point representation (Jacobian coordinates)
    struct ProjectivePoint {
        BIGNUM* x;
        BIGNUM* y;
        BIGNUM* z;
        bool is_infinity;
        
        ProjectivePoint();
        ProjectivePoint(const ProjectivePoint& other);
        ~ProjectivePoint();
        ProjectivePoint& operator=(const ProjectivePoint& other);
        ProjectivePoint(ProjectivePoint&& other) noexcept;
        ProjectivePoint& operator=(ProjectivePoint&& other) noexcept;
    };
    
    // Get curve parameters
    BIGNUM* getP() const { return p_; }
    BIGNUM* getA() const { return a_; }
    BIGNUM* getB() const { return b_; }
    BIGNUM* getQ() const { return q_; }
    Point getG() const { return g_; }
    
    // Point operations in affine coordinates
    Point pointAdd(const Point& P, const Point& Q) const;
    Point pointDouble(const Point& P) const;
    Point scalarMult(const BIGNUM* k, const Point& P) const;
    
    // Point operations in projective coordinates (Jacobian)
    ProjectivePoint toProjective(const Point& P) const;
    Point toAffine(const ProjectivePoint& P) const;
    ProjectivePoint pointAddProjective(const ProjectivePoint& P, const ProjectivePoint& Q) const;
    ProjectivePoint pointDoubleProjective(const ProjectivePoint& P) const;
    ProjectivePoint scalarMultProjective(const BIGNUM* k, const Point& P) const;
    
    // Utility functions
    bool isOnCurve(const Point& P) const;
    bool isOnCurve(const ProjectivePoint& P) const;
    
private:
    EllipticCurve();
    ~EllipticCurve();
    EllipticCurve(const EllipticCurve&) = delete;
    EllipticCurve& operator=(const EllipticCurve&) = delete;
    
    // Curve parameters
    BIGNUM* p_;  // prime modulus
    BIGNUM* a_;  // curve parameter a
    BIGNUM* b_;  // curve parameter b
    BIGNUM* q_;  // order of G
    Point g_;    // generator point
    
    // Helper for modular arithmetic
    BIGNUM* mod(const BIGNUM* a) const;
    BIGNUM* modAdd(const BIGNUM* a, const BIGNUM* b) const;
    BIGNUM* modSub(const BIGNUM* a, const BIGNUM* b) const;
    BIGNUM* modMul(const BIGNUM* a, const BIGNUM* b) const;
    BIGNUM* modInv(const BIGNUM* a) const;
};

#endif // ELLIPTIC_CURVE_HPP