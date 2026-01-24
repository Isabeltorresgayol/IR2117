#include <iostream>

int computeSum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}

int main() {
    int number;

    std::cout << "Enter a number: ";
    std::cin >> number;

    if (number >= 1) {
        std::cout << "Sum = " << computeSum(number) << std::endl;
    } else {
        std::cout << "Number must be at least 1. Program finished." << std::endl;
    }

    return 0;
}

