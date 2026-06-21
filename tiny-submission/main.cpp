#include "LinearSystem.hpp"
#include "Regression.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void TestGaussianElimination() {
    Matrix A(3, 3);

    A(1, 1) = 3.0;
    A(1, 2) = 2.0;
    A(1, 3) = -1.0;

    A(2, 1) = 2.0;
    A(2, 2) = -2.0;
    A(2, 3) = 4.0;

    A(3, 1) = -1.0;
    A(3, 2) = 0.5;
    A(3, 3) = -1.0;

    Vector b(3);
    b(1) = 1.0;
    b(2) = -2.0;
    b(3) = 0.0;

    LinearSystem system(A, b);
    Vector solution = system.Solve();

    cout << "Gaussian elimination solution: " << solution << "\n";
    cout << "det(A): " << A.Determinant() << "\n";
    cout << "A inverse:\n" << A.Inverse() << "\n\n";
}

void TestConjugateGradient() {
    Matrix A(3, 3);

    A(1, 1) = 4.0;
    A(1, 2) = 1.0;
    A(1, 3) = 0.0;

    A(2, 1) = 1.0;
    A(2, 2) = 3.0;
    A(2, 3) = 1.0;

    A(3, 1) = 0.0;
    A(3, 2) = 1.0;
    A(3, 3) = 2.0;

    Vector b(3);
    b(1) = 1.0;
    b(2) = 2.0;
    b(3) = 3.0;

    PosSymLinSystem system(A, b);
    Vector solution = system.Solve();

    cout << "Conjugate gradient solution: " << solution << "\n\n";
}

void TestPseudoInverse() {
    Matrix A(4, 2);

    A(1, 1) = 1.0;
    A(1, 2) = 1.0;

    A(2, 1) = 1.0;
    A(2, 2) = 2.0;

    A(3, 1) = 1.0;
    A(3, 2) = 3.0;

    A(4, 1) = 1.0;
    A(4, 2) = 4.0;

    Vector b(4);
    b(1) = 6.0;
    b(2) = 5.0;
    b(3) = 7.0;
    b(4) = 10.0;

    Vector solution = A.PseudoInverse() * b;

    cout << "Least-squares solution: " << solution << "\n\n";
}

void TestRegression(const string& dataPath) {
    if (!filesystem::exists(dataPath)) {
        std::cout << "Data file not found: " << dataPath << "\n";
        return;
    }

    vector<HardwareRecord> records = LoadHardwareData(dataPath);
    CpuRegressionModel model;
    RegressionResult result = model.FitAndEvaluate(records);

    cout << "CPU performance regression\n";
    cout << "Records: " << records.size() << "\n";
    cout << "Training count: " << result.trainingCount << "\n";
    cout << "Testing count: " << result.testingCount << "\n";
    cout << "Parameters: " << result.parameters << "\n";
    cout << "RMSE: " << result.rmse << "\n\n";
}

int main() {
    try {
        TestGaussianElimination();
        TestConjugateGradient();
        TestPseudoInverse();
        TestRegression("data/machine.data");

        return 0;
    } catch (const std::exception& error) {
        cerr << "Error: " << error.what() << "\n";
        return 1;
    }
}