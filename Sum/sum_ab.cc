#include <iostream>

int main() {
    int a, b;
    int sum = 0;

    do {
        std::cout << "Enter first number (>=1): ";
        std::cin >> a;
    } while (a < 1);

    std::cout << "Enter second number: ";
    std::cin >> b;

    for (int i = a; i <= b; i++) {
        sum += i;
    }

    std::cout << "Sum = " << sum << std::endl;

    return 0;
}

