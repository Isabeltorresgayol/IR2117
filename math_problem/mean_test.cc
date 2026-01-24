#include <iostream>
#include <vector>

int main() {
    int n;
    do {
        std::cout << "Enter the number of data points (>=1): ";
        std::cin >> n;
    } while (n < 1);

    std::vector<double> data(n);
    double sum = 0;

    for (int i = 0; i < n; i++) {
        std::cout << "Enter data point " << i + 1 << ": ";
        std::cin >> data[i];
        sum += data[i];
    }

    double mean = sum / n;
    std::cout << "Sample mean = " << mean << std::endl;

    return 0;
}

