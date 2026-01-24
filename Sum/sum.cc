#include <iostream>

int main() {
    int number;
    int sum = 0;

    std::cout << "Enter a number: ";
    std::cin >> number;

    for (int i = 1; i <= number; i++) {
        sum += i;
    }

    std::cout << "Sum = " << sum << std::endl;

    return 0;
}

