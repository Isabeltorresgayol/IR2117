#include <iostream>

int main() {
    int a, b;
    int sum = 0;

    do {
        std::cout << "Enter first number (>=1): ";
        std::cin >> a;
    } while (a < 1);

    do {
        std::cout << "Enter second number (>=1): ";
        std::cin >> b;
    } while (b < 1);

    for (int i = a; i <= b; i++) {
        sum += i;
    }

    std::cout << "Sum = " << sum << std::endl;

    return 0;
}

