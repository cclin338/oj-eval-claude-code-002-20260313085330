#include "code.cpp"
#include <iostream>

void test_mult(const std::string &a_str, const std::string &b_str, const std::string &expected) {
    sjtu::int2048 a(a_str);
    sjtu::int2048 b(b_str);
    sjtu::int2048 c = a * b;

    std::cout << a << " * " << b << " = " << c << std::endl;
    if (c == sjtu::int2048(expected)) {
        std::cout << "✓ Correct" << std::endl;
    } else {
        std::cout << "✗ WRONG! Expected: " << expected << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    test_mult("2", "3", "6");
    test_mult("123", "456", "56088");
    test_mult("999", "999", "998001");
    test_mult("12345678", "87654321", "1082152022374638");
    test_mult("-123", "456", "-56088");
    test_mult("123", "-456", "-56088");
    test_mult("-123", "-456", "56088");

    // Large numbers
    test_mult("123456789012345678901234567890", "987654321098765432109876543210",
              "121932631137021795226185032733622923332237463801111263526900");

    return 0;
}
