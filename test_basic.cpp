#include "src/include/int2048.h"
#include <iostream>

int main() {
    // Test constructors
    sjtu::int2048 a;
    a.print();
    std::cout << std::endl;

    sjtu::int2048 b((long long)1145141919810);
    b.print();
    std::cout << std::endl;

    sjtu::int2048 c((long long)-1145141919810);
    c.print();
    std::cout << std::endl;

    sjtu::int2048 d("-2333333333333333333333333333333333333333333333333333333");
    d.print();
    std::cout << std::endl;

    sjtu::int2048 e("19260817192608171926081719260817192608171926081719260817");
    e.print();
    std::cout << std::endl;

    // Test read
    d.read("-2333333333333333333333333333333333333333333333333333333");
    d.print();
    std::cout << std::endl;

    d.read("19260817192608171926081719260817192608171926081719260817");
    d.print();
    std::cout << std::endl;

    sjtu::int2048 zero("0");
    zero.print();
    std::cout << std::endl;

    sjtu::int2048 negzero("-0");
    negzero.print();
    std::cout << std::endl;

    sjtu::int2048 f(d);
    f.print();
    std::cout << std::endl;

    // Test addition
    sjtu::int2048 x("123");
    sjtu::int2048 y("456");
    x.add(y);
    x.print();
    std::cout << std::endl;

    return 0;
}
