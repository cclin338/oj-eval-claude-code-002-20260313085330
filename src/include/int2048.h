#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

namespace sjtu {

class int2048 {
private:
    std::vector<int> digits; // Store in base 1e9, least significant first
    bool negative;           // Sign flag

    void remove_leading_zeros();
    int compare_abs(const int2048 &other) const;
    void add_abs(const int2048 &other);
    void subtract_abs(const int2048 &other);

public:
    // Constructors
    int2048();
    int2048(long long);
    int2048(const std::string &);
    int2048(const int2048 &);

    // Integer1 functions
    void read(const std::string &);
    void print();
    int2048 &add(const int2048 &);
    friend int2048 add(int2048, const int2048 &);
    int2048 &minus(const int2048 &);
    friend int2048 minus(int2048, const int2048 &);

    // Integer2 operators
    int2048 operator+() const;
    int2048 operator-() const;
    int2048 &operator=(const int2048 &);
    int2048 &operator+=(const int2048 &);
    friend int2048 operator+(int2048, const int2048 &);
    int2048 &operator-=(const int2048 &);
    friend int2048 operator-(int2048, const int2048 &);
    int2048 &operator*=(const int2048 &);
    friend int2048 operator*(int2048, const int2048 &);
    int2048 &operator/=(const int2048 &);
    friend int2048 operator/(int2048, const int2048 &);
    int2048 &operator%=(const int2048 &);
    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);
    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);
    friend bool operator!=(const int2048 &, const int2048 &);
    friend bool operator<(const int2048 &, const int2048 &);
    friend bool operator>(const int2048 &, const int2048 &);
    friend bool operator<=(const int2048 &, const int2048 &);
    friend bool operator>=(const int2048 &, const int2048 &);
};

} // namespace sjtu

#endif
