#include "Regression.hpp"

#include "Matrix.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <random>
#include <sstream>
#include <stdexcept>

RegressionResult::RegressionResult()
    : parameters(6), rmse(0.0), trainingCount(0), testingCount(0) {}

CpuRegressionModel::CpuRegressionModel(double trainRatio, double regularization)
    : mTrainRatio(trainRatio), mRegularization(regularization) {
    if (trainRatio <= 0.0 || trainRatio >= 1.0) {
        throw std::invalid_argument("Training ratio must be between 0 and 1");
    }

    if (regularization < 0.0) {
        throw std::invalid_argument("Regularization must be non-negative");
    }
}

std::vector<std::string> CpuRegressionModel::SplitCsvLine(const std::string& line) {
    std::vector<std::string> values;
    std::stringstream stream(line);
    std::string item;

    while (std::getline(stream, item, ',')) {
        values.push_back(item);
    }

    return values;
}

double CpuRegressionModel::FeatureValue(const HardwareRecord& record, int index) {
    switch (index) {
        case 1:
            return record.myct;
        case 2:
            return record.mmin;
        case 3:
            return record.mmax;
        case 4:
            return record.cach;
        case 5:
            return record.chmin;
        case 6:
            return record.chmax;
        default:
            throw std::invalid_argument("Invalid feature index");
    }
}

std::vector<HardwareRecord> CpuRegressionModel::LoadHardwareData(const std::string& path) {
    std::ifstream input(path);

    if (!input) {
        throw std::runtime_error("Unable to open data file: " + path);
    }

    std::vector<HardwareRecord> records;
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> fields = SplitCsvLine(line);

        if (fields.size() != 10) {
            continue;
        }

        HardwareRecord record;
        record.vendor = fields[0];
        record.model = fields[1];
        record.myct = std::stod(fields[2]);
        record.mmin = std::stod(fields[3]);
        record.mmax = std::stod(fields[4]);
        record.cach = std::stod(fields[5]);
        record.chmin = std::stod(fields[6]);
        record.chmax = std::stod(fields[7]);
        record.prp = std::stod(fields[8]);
        record.erp = std::stod(fields[9]);

        records.push_back(record);
    }

    return records;
}

RegressionResult CpuRegressionModel::FitAndEvaluate(const std::vector<HardwareRecord>& records) const {
    if (records.size() < 10) {
        throw std::invalid_argument("Not enough records for train-test evaluation");
    }

    std::vector<HardwareRecord> shuffled = records;
    std::mt19937 generator(42);
    std::shuffle(shuffled.begin(), shuffled.end(), generator);

    int trainingCount = static_cast<int>(std::floor(shuffled.size() * mTrainRatio));
    int testingCount = static_cast<int>(shuffled.size()) - trainingCount;

    if (trainingCount <= 0 || testingCount <= 0) {
        throw std::runtime_error("Invalid train-test split");
    }

    Matrix designMatrix(trainingCount, mFeatureCount);
    Vector target(trainingCount);

    for (int i = 1; i <= trainingCount; i++) {
        const HardwareRecord& record = shuffled[i - 1];

        for (int j = 1; j <= mFeatureCount; j++) {
            designMatrix(i, j) = FeatureValue(record, j);
        }

        target(i) = record.prp;
    }

    Vector parameters = designMatrix.PseudoInverse(mRegularization) * target;

    double squaredErrorSum = 0.0;

    for (int i = 0; i < testingCount; i++) {
        const HardwareRecord& record = shuffled[trainingCount + i];
        double prediction = Predict(record, parameters);
        double error = prediction - record.prp;
        squaredErrorSum += error * error;
    }

    RegressionResult result;
    result.parameters = parameters;
    result.rmse = std::sqrt(squaredErrorSum / testingCount);
    result.trainingCount = trainingCount;
    result.testingCount = testingCount;

    return result;
}

double CpuRegressionModel::Predict(const HardwareRecord& record, const Vector& parameters) const {
    if (parameters.Size() != mFeatureCount) {
        throw std::invalid_argument("Regression parameter vector has invalid size");
    }

    double prediction = 0.0;

    for (int j = 1; j <= mFeatureCount; j++) {
        prediction += parameters(j) * FeatureValue(record, j);
    }

    return prediction;
}

std::vector<HardwareRecord> LoadHardwareData(const std::string& path) {
    return CpuRegressionModel::LoadHardwareData(path);
}

RegressionResult RunCpuRegression(const std::vector<HardwareRecord>& records,
                                  double trainRatio,
                                  double regularization) {
    CpuRegressionModel model(trainRatio, regularization);
    return model.FitAndEvaluate(records);
}