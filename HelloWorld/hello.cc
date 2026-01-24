#include <iostream>
#include <string>

int main()
{
    std::string name;

    std::cout << "What is your name?" << std::endl;  // Line break here
    std::getline(std::cin, name);

    std::cout << "Hello, " << name << "!" << std::endl;

    return 0;
}

