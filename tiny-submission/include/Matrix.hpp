#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vector.hpp"

#include <iosfwd>

class Matrix {
private:
    int mNumRows;
    int mNumCols;
    double** mData;

    void Allocate(int numRows, int numCols);
    void Deallocate();

public:
    Matrix() = delete;
    Matrix(int numRows, int numCols);
    Matrix(const Matrix& other);
    ~Matrix();

    Matrix& operator=(const Matrix& other);

    int row() const;
    int col() const;
    int GetNumberOfRows() const;
    int GetNumberOfColumns() const;

    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;

    Matrix operator+() const;
    Matrix operator-() const;

    Matrix operator+(const Matrix& rhs) const;
    Matrix operator-(const Matrix& rhs) const;
    Matrix operator*(const Matrix& rhs) const;
    Vector operator*(const Vector& rhs) const;
    Matrix operator*(double scalar) const;

    Matrix& operator+=(const Matrix& rhs);
    Matrix& operator-=(const Matrix& rhs);
    Matrix& operator*=(double scalar);

    Matrix Transpose() const;
    double Determinant() const;
    Matrix Inverse() const;
    Matrix PseudoInverse(double regularization = 1e-8) const;

    static Matrix Identity(int size);
};

Matrix operator*(double scalar, const Matrix& matrix);
std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

#endif