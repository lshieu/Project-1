#include "Vector.hpp"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

Vector::Vector(int size)
    : mSize(size), mData(nullptr) {
    if (size <= 0) {
        throw std::invalid_argument("Vector size must be positive");
    }

    mData = new double[mSize];

    for (int i = 0; i < mSize; i++) {
        mData[i] = 0.0;
    }
}

Vector::Vector(const Vector& other)
    : mSize(other.mSize), mData(new double[other.mSize]) {
    for (int i = 0; i < mSize; i++) {
        mData[i] = other.mData[i];
    }
}

Vector::~Vector() {
    delete[] mData;
}

Vector& Vector::operator=(const Vector& other) {
    if (this == &other) {
        return *this;
    }

    double* newData = new double[other.mSize];

    for (int i = 0; i < other.mSize; i++) {
        newData[i] = other.mData[i];
    }

    delete[] mData;

    mSize = other.mSize;
    mData = newData;

    return *this;
}

int Vector::Size() const {
    return mSize;
}

double& Vector::operator[](int index) {
    assert(index >= 0 && index < mSize);
    return mData[index];
}

const double& Vector::operator[](int index) const {
    assert(index >= 0 && index < mSize);
    return mData[index];
}

double& Vector::operator()(int index) {
    assert(index >= 1 && index <= mSize);
    return mData[index - 1];
}

const double& Vector::operator()(int index) const {
    assert(index >= 1 && index <= mSize);
    return mData[index - 1];
}

Vector Vector::operator+() const {
    return *this;
}

Vector Vector::operator-() const {
    Vector result(mSize);

    for (int i = 0; i < mSize; i++) {
        result.mData[i] = -mData[i];
    }

    return result;
}

Vector Vector::operator+(const Vector& rhs) const {
    assert(mSize == rhs.mSize);

    Vector result(mSize);

    for (int i = 0; i < mSize; i++) {
        result.mData[i] = mData[i] + rhs.mData[i];
    }

    return result;
}

Vector Vector::operator-(const Vector& rhs) const {
    assert(mSize == rhs.mSize);

    Vector result(mSize);

    for (int i = 0; i < mSize; i++) {
        result.mData[i] = mData[i] - rhs.mData[i];
    }

    return result;
}

Vector Vector::operator*(double scalar) const {
    Vector result(mSize);

    for (int i = 0; i < mSize; i++) {
        result.mData[i] = mData[i] * scalar;
    }

    return result;
}

Vector& Vector::operator+=(const Vector& rhs) {
    assert(mSize == rhs.mSize);

    for (int i = 0; i < mSize; i++) {
        mData[i] += rhs.mData[i];
    }

    return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
    assert(mSize == rhs.mSize);

    for (int i = 0; i < mSize; i++) {
        mData[i] -= rhs.mData[i];
    }

    return *this;
}

Vector& Vector::operator*=(double scalar) {
    for (int i = 0; i < mSize; i++) {
        mData[i] *= scalar;
    }

    return *this;
}

double Vector::Dot(const Vector& rhs) const {
    assert(mSize == rhs.mSize);

    double sum = 0.0;

    for (int i = 0; i < mSize; i++) {
        sum += mData[i] * rhs.mData[i];
    }

    return sum;
}

double Vector::Norm() const {
    return std::sqrt(Dot(*this));
}

Vector operator*(double scalar, const Vector& vector) {
    return vector * scalar;
}

std::ostream& operator<<(std::ostream& os, const Vector& vector) {
    os << "[";

    for (int i = 0; i < vector.Size(); i++) {
        os << std::fixed << std::setprecision(6) << vector[i];

        if (i + 1 < vector.Size()) {
            os << ", ";
        }
    }

    os << "]";
    return os;
}