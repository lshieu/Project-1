#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iosfwd>

class Vector {
private:
    int mSize;
    double* mData;

public:
    Vector() = delete;
    explicit Vector(int size);
    Vector(const Vector& other);
    ~Vector();

    Vector& operator=(const Vector& other);

    int Size() const;

    double& operator[](int index);
    const double& operator[](int index) const;

    double& operator()(int index);
    const double& operator()(int index) const;

    Vector operator+() const;
    Vector operator-() const;

    Vector operator+(const Vector& rhs) const;
    Vector operator-(const Vector& rhs) const;
    Vector operator*(double scalar) const;

    Vector& operator+=(const Vector& rhs);
    Vector& operator-=(const Vector& rhs);
    Vector& operator*=(double scalar);

    double Dot(const Vector& rhs) const;
    double Norm() const;
};

Vector operator*(double scalar, const Vector& vector);
std::ostream& operator<<(std::ostream& os, const Vector& vector);

#endif