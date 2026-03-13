#include "code.cpp"
#include <iostream>

int main() {
    sjtu::int2048 a("-10");
    sjtu::int2048 b("3");

    std::cout << "Computing -10 / 3:" << std::endl;
    std::cout << "Expected: -4" << std::endl;

    sjtu::int2048 q = a / b;
    std::cout << "Got: " << q << std::endl;

    sjtu::int2048 r = a % b;
    std::cout << "Remainder: " << r << std::endl;
    std::cout << "Expected remainder: 2" << std::endl;

    // Python check
    std::cout << "\nPython: -10 // 3 = -4, -10 % 3 = 2" << std::endl;
    std::cout << "Check: -4 * 3 + 2 = " << (-4 * 3 + 2) << std::endl;

    return 0;
}
