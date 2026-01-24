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

    do {
        std::cout << "Enter a number (>=1): ";
        std::cin >> number;
    } while (number < 1);

   std::cout << "Sum = " << computeSum(number) << std::endl;

    return 0;
}

