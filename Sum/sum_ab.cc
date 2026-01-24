#include <iostream>

int main() {
    int a, b;
    int sum = 0;

    do {
        std::cout << "Enter first number (>=1): ";
        std::cin >> a;
    } while (a < 1);

    do {
        std::cout << "Enter second number (>=1 and >= first number): ";
        std::cin >> b;
    } while (b < 1 || b < a);

    for (int i = a; i <= b; i++) {
        sum += i;
    }

    std::cout << "Sum from " << a << " to " << b << " is: " << sum << std::endl;

    return 0;
}

