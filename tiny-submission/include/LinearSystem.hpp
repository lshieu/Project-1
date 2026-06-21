#ifndef LINEAR_SYSTEM_HPP
#define LINEAR_SYSTEM_HPP

#include "Matrix.hpp"
#include "Vector.hpp"

class LinearSystem {
protected:
    int mSize;
    Matrix* mpA;
    Vector* mpb;

public:
    LinearSystem() = delete;
    LinearSystem(const Matrix& A, const Vector& b);
    LinearSystem(const LinearSystem& other) = delete;
    LinearSystem& operator=(const LinearSystem& other) = delete;
    virtual ~LinearSystem();

    virtual Vector Solve() const;
};

class PosSymLinSystem : public LinearSystem {
private:
    bool IsSymmetric(double tolerance = 1e-10) const;

public:
    PosSymLinSystem(const Matrix& A, const Vector& b);

    Vector Solve() const override;
};

#endif