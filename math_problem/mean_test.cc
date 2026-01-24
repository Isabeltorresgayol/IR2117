#include <iostream>
#include <vector>

double computeMean(const std::vector<double>& data) {
    double sum = 0;
    for (double x : data) sum += x;
    return sum / data.size();
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
    std::cout << "Sample mean = " << mean << std::endl;

    double mu0;
    std::cout << "Enter a hypothetical value μ₀: ";
    std::cin >> mu0;

    if (mean > mu0) std::cout << "The sample mean is greater than μ₀" << std::endl;
    else if (mean < mu0) std::cout << "The sample mean is less than μ₀" << std::endl;
    else std::cout << "The sample mean is equal to μ₀" << std::endl;

    return 0;
}

