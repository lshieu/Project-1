#ifndef REGRESSION_HPP
#define REGRESSION_HPP

#include "Vector.hpp"

#include <string>
#include <vector>

struct HardwareRecord {
    std::string vendor;
    std::string model;
    double myct;
    double mmin;
    double mmax;
    double cach;
    double chmin;
    double chmax;
    double prp;
    double erp;
};

struct RegressionResult {
    Vector parameters;
    double rmse;
    int trainingCount;
    int testingCount;

    RegressionResult();
};

class CpuRegressionModel {
private:
    double mTrainRatio;
    double mRegularization;

    static const int mFeatureCount = 6;

    static std::vector<std::string> SplitCsvLine(const std::string& line);
    static double FeatureValue(const HardwareRecord& record, int index);

public:
    explicit CpuRegressionModel(double trainRatio = 0.80,
                                double regularization = 1e-8);

    RegressionResult FitAndEvaluate(const std::vector<HardwareRecord>& records) const;
    double Predict(const HardwareRecord& record, const Vector& parameters) const;

    static std::vector<HardwareRecord> LoadHardwareData(const std::string& path);
};

std::vector<HardwareRecord> LoadHardwareData(const std::string& path);
RegressionResult RunCpuRegression(const std::vector<HardwareRecord>& records,
                                  double trainRatio = 0.80,
                                  double regularization = 1e-8);

#endif