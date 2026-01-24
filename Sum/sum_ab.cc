#include <iostream>

int main() {
    int a, b;
    int sum = 0;

    std::cout << "Enter first number: ";
    std::cin >> a;

    std::cout << "Enter second number: ";
    std::cin >> b;

    for (int i = a; i <= b; i++) {
        sum += i;
    }

    std::cout << "Sum from " << a << " to " << b << " is: " << sum << std::endl;

    return 0;
}

