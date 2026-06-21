# Tiny Project 1: Linear System and CPU Regression

## Overview

This project implements basic numerical linear algebra classes in C++ and uses them to solve linear systems and run a linear regression experiment on the Computer Hardware dataset.

The submission covers:

- `Vector` class with manual dynamic memory management
- `Matrix` class with manual dynamic memory management
- Gaussian elimination with partial pivoting
- Determinant and matrix inverse calculation
- Regularized pseudo-inverse using Tikhonov regularization through normal equations
- `LinearSystem` base class
- `PosSymLinSystem` derived class using the conjugate gradient method
- Linear regression for CPU relative performance prediction
- 80 percent training and 20 percent testing split
- RMSE evaluation

## Project Structure

```text
.
├── Makefile
├── README.md
├── main.cpp
├── include/
│   ├── Vector.hpp
│   ├── Matrix.hpp
│   ├── LinearSystem.hpp
│   └── Regression.hpp
├── src/
│   ├── Vector.cpp
│   ├── Matrix.cpp
│   ├── LinearSystem.cpp
│   └── Regression.cpp
└── data/
    └── machine.data
```

## Main Components

### Vector

The `Vector` class stores a one-dimensional dynamic array of `double` values.

Main features:

- Constructor and destructor
- Copy constructor
- Assignment operator
- Zero-based indexing with `operator[]`
- One-based indexing with `operator()`
- Vector addition and subtraction
- Scalar multiplication
- Dot product
- Euclidean norm
- Output stream operator

### Matrix

The `Matrix` class stores a two-dimensional dynamic array of `double` values.

Main features:

- Constructor and destructor
- Copy constructor
- Assignment operator
- One-based indexing with `operator()`
- Matrix addition and subtraction
- Matrix multiplication
- Matrix-vector multiplication
- Scalar multiplication
- Transpose
- Determinant
- Inverse
- Regularized pseudo-inverse
- Identity matrix helper
- Output stream operator

The pseudo-inverse is implemented using Tikhonov regularization through regularized normal equations:

- If the matrix has more rows than columns, it computes `(A^T A + λI)^-1 A^T`.
- If the matrix has more columns than rows, it computes `A^T (A A^T + λI)^-1`.

The small regularization value helps avoid failure when the normal matrix is close to singular.

### LinearSystem

The `LinearSystem` class represents a square linear system:

```text
Ax = b
```

It stores copies of the matrix `A` and vector `b`. Its `Solve()` method uses Gaussian elimination with partial pivoting.

### PosSymLinSystem

`PosSymLinSystem` derives from `LinearSystem` and overrides `Solve()`.

It is designed for symmetric positive definite systems. The class checks whether the matrix is symmetric, then solves the system using the conjugate gradient method.

The implementation does not check positive definiteness because that is not required by the project specification.

### CPU Regression

The regression module reads `data/machine.data` and predicts published relative CPU performance, `PRP`, using these six features:

```text
MYCT, MMIN, MMAX, CACH, CHMIN, CHMAX
```

The model has the form:

```text
PRP = x1*MYCT + x2*MMIN + x3*MMAX + x4*CACH + x5*CHMIN + x6*CHMAX
```

The dataset is shuffled using a fixed random seed for reproducible results. Then it is split into:

- 80 percent training data
- 20 percent testing data

The model parameters are calculated using the regularized pseudo-inverse. The testing error is reported using RMSE.

## Build and Run

The project can be built using the provided Makefile.

```bash
make
make run
```

On Windows with MinGW, use:

```powershell
mingw32-make
mingw32-make run
```

To remove the compiled executable:

```bash
make clean
```

The dataset file should be located at:

```text
data/machine.data
```