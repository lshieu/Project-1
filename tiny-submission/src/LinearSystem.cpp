#include "LinearSystem.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

LinearSystem::LinearSystem(const Matrix& A, const Vector& b)
    : mSize(0), mpA(nullptr), mpb(nullptr) {
    if (A.GetNumberOfRows() != A.GetNumberOfColumns()) {
        throw std::invalid_argument("LinearSystem requires a square matrix");
    }

    if (A.GetNumberOfRows() != b.Size()) {
        throw std::invalid_argument("Matrix and vector sizes do not match");
    }

    mSize = A.GetNumberOfRows();
    mpA = new Matrix(A);
    mpb = new Vector(b);
}

LinearSystem::~LinearSystem() {
    delete mpA;
    delete mpb;
}

Vector LinearSystem::Solve() const {
    Matrix a(*mpA);
    Vector b(*mpb);

    for (int k = 1; k <= mSize; k++) {
        int pivotRow = k;
        double maxValue = std::fabs(a(k, k));

        for (int i = k + 1; i <= mSize; i++) {
            double candidate = std::fabs(a(i, k));

            if (candidate > maxValue) {
                maxValue = candidate;
                pivotRow = i;
            }
        }

        if (maxValue < 1e-14) {
            throw std::runtime_error("Linear system is singular or nearly singular");
        }

        if (pivotRow != k) {
            for (int j = k; j <= mSize; j++) {
                std::swap(a(k, j), a(pivotRow, j));
            }

            std::swap(b(k), b(pivotRow));
        }

        for (int i = k + 1; i <= mSize; i++) {
            double factor = a(i, k) / a(k, k);
            a(i, k) = 0.0;

            for (int j = k + 1; j <= mSize; j++) {
                a(i, j) -= factor * a(k, j);
            }

            b(i) -= factor * b(k);
        }
    }

    Vector x(mSize);

    for (int i = mSize; i >= 1; i--) {
        double sum = b(i);

        for (int j = i + 1; j <= mSize; j++) {
            sum -= a(i, j) * x(j);
        }

        x(i) = sum / a(i, i);
    }

    return x;
}

bool PosSymLinSystem::IsSymmetric(double tolerance) const {
    for (int i = 1; i <= mSize; i++) {
        for (int j = i + 1; j <= mSize; j++) {
            if (std::fabs((*mpA)(i, j) - (*mpA)(j, i)) > tolerance) {
                return false;
            }
        }
    }

    return true;
}

PosSymLinSystem::PosSymLinSystem(const Matrix& A, const Vector& b)
    : LinearSystem(A, b) {
    if (!IsSymmetric()) {
        throw std::invalid_argument("Matrix must be symmetric");
    }
}

Vector PosSymLinSystem::Solve() const {
    const int maxIterations = 10000;
    const double tolerance = 1e-10;

    Vector x(mSize);
    Vector r = *mpb - ((*mpA) * x);
    Vector p = r;
    double rsOld = r.Dot(r);

    if (std::sqrt(rsOld) < tolerance) {
        return x;
    }

    for (int iteration = 0; iteration < maxIterations; iteration++) {
        Vector Ap = (*mpA) * p;
        double denominator = p.Dot(Ap);

        if (std::fabs(denominator) < 1e-14) {
            throw std::runtime_error("Conjugate gradient failed because the denominator is too small");
        }

        double alpha = rsOld / denominator;

        x += alpha * p;
        r -= alpha * Ap;

        double rsNew = r.Dot(r);

        if (std::sqrt(rsNew) < tolerance) {
            return x;
        }

        p = r + (rsNew / rsOld) * p;
        rsOld = rsNew;
    }

    throw std::runtime_error("Conjugate gradient did not converge");
}