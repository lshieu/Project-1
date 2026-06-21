#include "Matrix.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void Matrix::Allocate(int numRows, int numCols) {
    if (numRows <= 0 || numCols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }

    mNumRows = numRows;
    mNumCols = numCols;
    mData = new double*[mNumRows];

    for (int i = 0; i < mNumRows; i++) {
        mData[i] = new double[mNumCols];

        for (int j = 0; j < mNumCols; j++) {
            mData[i][j] = 0.0;
        }
    }
}

void Matrix::Deallocate() {
    if (mData == nullptr) {
        return;
    }

    for (int i = 0; i < mNumRows; i++) {
        delete[] mData[i];
    }

    delete[] mData;
    mData = nullptr;
    mNumRows = 0;
    mNumCols = 0;
}

Matrix::Matrix(int numRows, int numCols)
    : mNumRows(0), mNumCols(0), mData(nullptr) {
    Allocate(numRows, numCols);
}

Matrix::Matrix(const Matrix& other)
    : mNumRows(0), mNumCols(0), mData(nullptr) {
    Allocate(other.mNumRows, other.mNumCols);

    for (int i = 0; i < mNumRows; i++) {
        for (int j = 0; j < mNumCols; j++) {
            mData[i][j] = other.mData[i][j];
        }
    }
}

Matrix::~Matrix() {
    Deallocate();
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this;
    }

    Matrix copy(other);
    std::swap(mNumRows, copy.mNumRows);
    std::swap(mNumCols, copy.mNumCols);
    std::swap(mData, copy.mData);

    return *this;
}

int Matrix::row() const {
    return mNumRows;
}

int Matrix::col() const {
    return mNumCols;
}

int Matrix::GetNumberOfRows() const {
    return mNumRows;
}

int Matrix::GetNumberOfColumns() const {
    return mNumCols;
}

double& Matrix::operator()(int row, int col) {
    assert(row >= 1 && row <= mNumRows);
    assert(col >= 1 && col <= mNumCols);

    return mData[row - 1][col - 1];
}

const double& Matrix::operator()(int row, int col) const {
    assert(row >= 1 && row <= mNumRows);
    assert(col >= 1 && col <= mNumCols);

    return mData[row - 1][col - 1];
}

Matrix Matrix::operator+() const {
    return *this;
}

Matrix Matrix::operator-() const {
    Matrix result(mNumRows, mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            result(i, j) = -(*this)(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator+(const Matrix& rhs) const {
    assert(mNumRows == rhs.mNumRows);
    assert(mNumCols == rhs.mNumCols);

    Matrix result(mNumRows, mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            result(i, j) = (*this)(i, j) + rhs(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix& rhs) const {
    assert(mNumRows == rhs.mNumRows);
    assert(mNumCols == rhs.mNumCols);

    Matrix result(mNumRows, mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            result(i, j) = (*this)(i, j) - rhs(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    assert(mNumCols == rhs.mNumRows);

    Matrix result(mNumRows, rhs.mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= rhs.mNumCols; j++) {
            double sum = 0.0;

            for (int k = 1; k <= mNumCols; k++) {
                sum += (*this)(i, k) * rhs(k, j);
            }

            result(i, j) = sum;
        }
    }

    return result;
}

Vector Matrix::operator*(const Vector& rhs) const {
    assert(mNumCols == rhs.Size());

    Vector result(mNumRows);

    for (int i = 1; i <= mNumRows; i++) {
        double sum = 0.0;

        for (int j = 1; j <= mNumCols; j++) {
            sum += (*this)(i, j) * rhs(j);
        }

        result(i) = sum;
    }

    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(mNumRows, mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }

    return result;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
    assert(mNumRows == rhs.mNumRows);
    assert(mNumCols == rhs.mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            (*this)(i, j) += rhs(i, j);
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs) {
    assert(mNumRows == rhs.mNumRows);
    assert(mNumCols == rhs.mNumCols);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            (*this)(i, j) -= rhs(i, j);
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            (*this)(i, j) *= scalar;
        }
    }

    return *this;
}

Matrix Matrix::Transpose() const {
    Matrix result(mNumCols, mNumRows);

    for (int i = 1; i <= mNumRows; i++) {
        for (int j = 1; j <= mNumCols; j++) {
            result(j, i) = (*this)(i, j);
        }
    }

    return result;
}

double Matrix::Determinant() const {
    if (mNumRows != mNumCols) {
        throw std::invalid_argument("Determinant requires a square matrix");
    }

    Matrix a(*this);
    double determinant = 1.0;
    int sign = 1;

    for (int k = 1; k <= mNumRows; k++) {
        int pivotRow = k;
        double maxValue = std::fabs(a(k, k));

        for (int i = k + 1; i <= mNumRows; i++) {
            double candidate = std::fabs(a(i, k));

            if (candidate > maxValue) {
                maxValue = candidate;
                pivotRow = i;
            }
        }

        if (maxValue < 1e-14) {
            return 0.0;
        }

        if (pivotRow != k) {
            for (int j = 1; j <= mNumRows; j++) {
                std::swap(a(k, j), a(pivotRow, j));
            }

            sign *= -1;
        }

        for (int i = k + 1; i <= mNumRows; i++) {
            double factor = a(i, k) / a(k, k);

            for (int j = k; j <= mNumRows; j++) {
                a(i, j) -= factor * a(k, j);
            }
        }

        determinant *= a(k, k);
    }

    return sign * determinant;
}

Matrix Matrix::Inverse() const {
    if (mNumRows != mNumCols) {
        throw std::invalid_argument("Inverse requires a square matrix");
    }

    Matrix a(*this);
    Matrix inverse = Matrix::Identity(mNumRows);

    for (int k = 1; k <= mNumRows; k++) {
        int pivotRow = k;
        double maxValue = std::fabs(a(k, k));

        for (int i = k + 1; i <= mNumRows; i++) {
            double candidate = std::fabs(a(i, k));

            if (candidate > maxValue) {
                maxValue = candidate;
                pivotRow = i;
            }
        }

        if (maxValue < 1e-14) {
            throw std::runtime_error("Matrix is singular and cannot be inverted");
        }

        if (pivotRow != k) {
            for (int j = 1; j <= mNumRows; j++) {
                std::swap(a(k, j), a(pivotRow, j));
                std::swap(inverse(k, j), inverse(pivotRow, j));
            }
        }

        double pivot = a(k, k);

        for (int j = 1; j <= mNumRows; j++) {
            a(k, j) /= pivot;
            inverse(k, j) /= pivot;
        }

        for (int i = 1; i <= mNumRows; i++) {
            if (i == k) {
                continue;
            }

            double factor = a(i, k);

            for (int j = 1; j <= mNumRows; j++) {
                a(i, j) -= factor * a(k, j);
                inverse(i, j) -= factor * inverse(k, j);
            }
        }
    }

    return inverse;
}

Matrix Matrix::PseudoInverse(double regularization) const {
    if (regularization < 0.0) {
        throw std::invalid_argument("Regularization must be non-negative");
    }

    Matrix transpose = Transpose();

    if (mNumRows >= mNumCols) {
        Matrix normal = transpose * (*this);

        for (int i = 1; i <= normal.GetNumberOfRows(); i++) {
            normal(i, i) += regularization;
        }

        return normal.Inverse() * transpose;
    }

    Matrix normal = (*this) * transpose;

    for (int i = 1; i <= normal.GetNumberOfRows(); i++) {
        normal(i, i) += regularization;
    }

    return transpose * normal.Inverse();
}

Matrix Matrix::Identity(int size) {
    if (size <= 0) {
        throw std::invalid_argument("Identity matrix size must be positive");
    }

    Matrix identity(size, size);

    for (int i = 1; i <= size; i++) {
        identity(i, i) = 1.0;
    }

    return identity;
}

Matrix operator*(double scalar, const Matrix& matrix) {
    return matrix * scalar;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 1; i <= matrix.GetNumberOfRows(); i++) {
        os << "[";

        for (int j = 1; j <= matrix.GetNumberOfColumns(); j++) {
            os << std::fixed << std::setprecision(6) << matrix(i, j);

            if (j < matrix.GetNumberOfColumns()) {
                os << ", ";
            }
        }

        os << "]";

        if (i < matrix.GetNumberOfRows()) {
            os << "\n";
        }
    }

    return os;
}