#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

double computeMean(const std::vector<double>& data) {
    double sum = 0;
    for (double x : data) sum += x;
    return sum / data.size();
}

double computeStdDev(const std::vector<double>& data, double mean) {
    double sum = 0;
    for (double x : data)
        sum += (x - mean) * (x - mean);
    return std::sqrt(sum / data.size());
}

double computeMedian(std::vector<double> data) {
    std::sort(data.begin(), data.end());
    int n = data.size();
    if (n % 2 == 0)
        return (data[n/2 - 1] + data[n/2]) / 2;
    else
        return data[n/2];
}

int main() {
    int n;
    do {
        std::cout << "Enter the number of data points (>=1): ";
        std::cin >> n;
    } while (n < 1);

    std::vector<double> data(n);
    for (int i = 0; i < n; i++) {
        std::cout << "Enter data point " << i + 1 << ": ";
        std::cin >> data[i];
    }

    double mean = computeMean(data);
    double stddev = computeStdDev(data, mean);
    double median = computeMedian(data);

    std::cout << "Sample mean = " << mean << std::endl;
    std::cout << "Standard deviation = " << stddev << std::endl;
    std::cout << "Median = " << median << std::endl;

    double mu0;
    std::cout << "Enter a hypothetical value mu0: ";
    std::cin >> mu0;

    if (mean > mu0) std::cout << "The sample mean is greater than mu0" << std::endl;
    else if (mean < mu0) std::cout << "The sample mean is less than mu0" << std::endl;
    else std::cout << "The sample mean is equal to mu0" << std::endl;

    return 0;
}

