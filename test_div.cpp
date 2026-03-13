#include "code.cpp"
#include <iostream>

void test_div(const std::string &a_str, const std::string &b_str) {
    sjtu::int2048 a(a_str);
    sjtu::int2048 b(b_str);
    sjtu::int2048 q = a / b;
    sjtu::int2048 r = a % b;

    std::cout << a << " / " << b << " = " << q << std::endl;
    std::cout << a << " % " << b << " = " << r << std::endl;

    // Verify: a = q * b + r
    sjtu::int2048 check = q * b + r;
    std::cout << "Check: " << q << " * " << b << " + " << r << " = " << check << std::endl;
    if (check == a) {
        std::cout << "✓ Correct" << std::endl;
    } else {
        std::cout << "✗ WRONG!" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Test Python-style floor division
    test_div("10", "3");     // Should be 3
    test_div("-10", "3");    // Should be -4
    test_div("10", "-3");    // Should be -4
    test_div("-10", "-3");   // Should be 3

    test_div("7", "3");
    test_div("-7", "3");
    test_div("7", "-3");
    test_div("-7", "-3");

    // Edge cases
    test_div("0", "5");
    test_div("5", "10");
    test_div("-5", "10");

    return 0;
}
