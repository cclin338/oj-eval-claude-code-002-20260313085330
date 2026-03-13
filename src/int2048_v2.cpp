#include "include/int2048.h"
#include <algorithm>

namespace sjtu {

static const int BASE = 1000000000;
static const int BASE_DIGITS = 9;
static const int KARATSUBA_THRESHOLD = 32; // Use Karatsuba for numbers longer than this

void int2048::remove_leading_zeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) {
        negative = false;
    }
}

int int2048::compare_abs(const int2048 &other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size() ? -1 : 1;
    }
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i] ? -1 : 1;
        }
    }
    return 0;
}

void int2048::add_abs(const int2048 &other) {
    int carry = 0;
    for (size_t i = 0; i < std::max(digits.size(), other.digits.size()) || carry; i++) {
        if (i == digits.size()) digits.push_back(0);
        digits[i] += carry + (i < other.digits.size() ? other.digits[i] : 0);
        carry = digits[i] >= BASE;
        if (carry) digits[i] -= BASE;
    }
}

void int2048::subtract_abs(const int2048 &other) {
    int carry = 0;
    for (size_t i = 0; i < other.digits.size() || carry; i++) {
        digits[i] -= carry + (i < other.digits.size() ? other.digits[i] : 0);
        carry = digits[i] < 0;
        if (carry) digits[i] += BASE;
    }
    remove_leading_zeros();
}

// Karatsuba multiplication helper
static std::vector<int> karatsuba_multiply(const std::vector<int> &a, const std::vector<int> &b) {
    int n = a.size(), m = b.size();
    std::vector<int> result(n + m, 0);

    if (n <= KARATSUBA_THRESHOLD || m <= KARATSUBA_THRESHOLD) {
        // Use standard multiplication for small numbers
        for (int i = 0; i < n; i++) {
            long long carry = 0;
            for (int j = 0; j < m || carry; j++) {
                long long cur = result[i + j] +
                               (long long)a[i] * (j < m ? b[j] : 0) + carry;
                result[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        return result;
    }

    // Karatsuba algorithm
    int k = std::max(n, m) / 2;

    std::vector<int> a0(a.begin(), a.begin() + std::min(k, n));
    std::vector<int> a1(a.begin() + std::min(k, n), a.end());
    std::vector<int> b0(b.begin(), b.begin() + std::min(k, m));
    std::vector<int> b1(b.begin() + std::min(k, m), b.end());

    std::vector<int> z0 = karatsuba_multiply(a0, b0);
    std::vector<int> z2 = karatsuba_multiply(a1, b1);

    // Compute (a0 + a1) and (b0 + b1)
    std::vector<int> a_sum = a0;
    long long carry = 0;
    for (size_t i = 0; i < std::max(a_sum.size(), a1.size()) || carry; i++) {
        if (i == a_sum.size()) a_sum.push_back(0);
        a_sum[i] += carry + (i < a1.size() ? a1[i] : 0);
        carry = a_sum[i] >= BASE;
        if (carry) a_sum[i] -= BASE;
    }

    std::vector<int> b_sum = b0;
    carry = 0;
    for (size_t i = 0; i < std::max(b_sum.size(), b1.size()) || carry; i++) {
        if (i == b_sum.size()) b_sum.push_back(0);
        b_sum[i] += carry + (i < b1.size() ? b1[i] : 0);
        carry = b_sum[i] >= BASE;
        if (carry) b_sum[i] -= BASE;
    }

    std::vector<int> z1 = karatsuba_multiply(a_sum, b_sum);

    // z1 = z1 - z0 - z2
    carry = 0;
    for (size_t i = 0; i < std::max({z1.size(), z0.size(), z2.size()}) || carry; i++) {
        if (i == z1.size()) z1.push_back(0);
        z1[i] -= carry + (i < z0.size() ? z0[i] : 0) + (i < z2.size() ? z2[i] : 0);
        carry = z1[i] < 0;
        if (carry) z1[i] += BASE;
    }

    // Combine: result = z0 + z1 * BASE^k + z2 * BASE^(2k)
    result.assign(z0.size() + 2 * k, 0);
    for (size_t i = 0; i < z0.size(); i++) result[i] = z0[i];

    carry = 0;
    for (size_t i = 0; i < z1.size() || carry; i++) {
        if (k + i >= result.size()) result.push_back(0);
        result[k + i] += carry + (i < z1.size() ? z1[i] : 0);
        carry = result[k + i] >= BASE;
        if (carry) result[k + i] -= BASE;
    }

    carry = 0;
    for (size_t i = 0; i < z2.size() || carry; i++) {
        if (2 * k + i >= result.size()) result.push_back(0);
        result[2 * k + i] += carry + (i < z2.size() ? z2[i] : 0);
        carry = result[2 * k + i] >= BASE;
        if (carry) result[2 * k + i] -= BASE;
    }

    return result;
}

int2048::int2048() : negative(false) {
    digits.push_back(0);
}

int2048::int2048(long long num) : negative(num < 0) {
    if (num == 0) {
        digits.push_back(0);
        negative = false;
        return;
    }
    if (num < 0) num = -num;
    while (num > 0) {
        digits.push_back(num % BASE);
        num /= BASE;
    }
}

int2048::int2048(const std::string &s) {
    read(s);
}

int2048::int2048(const int2048 &other) : digits(other.digits), negative(other.negative) {}

void int2048::read(const std::string &s) {
    digits.clear();
    negative = false;

    int start = 0;
    if (!s.empty() && s[0] == '-') {
        negative = true;
        start = 1;
    }

    while (start < (int)s.length() && s[start] == '0') start++;

    if (start == (int)s.length()) {
        digits.push_back(0);
        negative = false;
        return;
    }

    for (int i = s.length(); i > start; i -= BASE_DIGITS) {
        int left = std::max(start, i - BASE_DIGITS);
        int val = 0;
        for (int j = left; j < i; j++) {
            val = val * 10 + (s[j] - '0');
        }
        digits.push_back(val);
    }
}

void int2048::print() {
    if (negative) std::cout << '-';
    std::cout << digits.back();
    for (int i = digits.size() - 2; i >= 0; i--) {
        int val = digits[i];
        for (int j = BASE / 10; j > 0; j /= 10) {
            std::cout << (val / j) % 10;
        }
    }
}

int2048 &int2048::add(const int2048 &other) {
    if (negative == other.negative) {
        add_abs(other);
    } else {
        int cmp = compare_abs(other);
        if (cmp == 0) {
            digits.clear();
            digits.push_back(0);
            negative = false;
        } else if (cmp > 0) {
            subtract_abs(other);
        } else {
            int2048 temp = other;
            temp.subtract_abs(*this);
            *this = temp;
        }
    }
    return *this;
}

int2048 add(int2048 a, const int2048 &b) {
    a.add(b);
    return a;
}

int2048 &int2048::minus(const int2048 &other) {
    if (negative != other.negative) {
        add_abs(other);
    } else {
        int cmp = compare_abs(other);
        if (cmp == 0) {
            digits.clear();
            digits.push_back(0);
            negative = false;
        } else if (cmp > 0) {
            subtract_abs(other);
        } else {
            int2048 temp = other;
            temp.subtract_abs(*this);
            temp.negative = !temp.negative;
            *this = temp;
        }
    }
    return *this;
}

int2048 minus(int2048 a, const int2048 &b) {
    a.minus(b);
    return a;
}

int2048 int2048::operator+() const {
    return *this;
}

int2048 int2048::operator-() const {
    int2048 result = *this;
    if (result.digits.size() != 1 || result.digits[0] != 0) {
        result.negative = !result.negative;
    }
    return result;
}

int2048 &int2048::operator=(const int2048 &other) {
    if (this != &other) {
        digits = other.digits;
        negative = other.negative;
    }
    return *this;
}

int2048 &int2048::operator+=(const int2048 &other) {
    return add(other);
}

int2048 &int2048::operator-=(const int2048 &other) {
    return minus(other);
}

int2048 &int2048::operator*=(const int2048 &other) {
    bool result_negative = negative != other.negative;

    std::vector<int> result = karatsuba_multiply(digits, other.digits);

    digits = result;
    negative = result_negative;
    remove_leading_zeros();
    return *this;
}

int2048 &int2048::operator/=(const int2048 &other) {
    bool dividend_negative = negative;
    bool divisor_negative = other.negative;
    bool result_negative = dividend_negative != divisor_negative;

    int2048 dividend = *this;
    dividend.negative = false;
    int2048 divisor = other;
    divisor.negative = false;

    if (dividend.compare_abs(divisor) < 0) {
        if (result_negative && (dividend.digits.size() > 1 || dividend.digits[0] != 0)) {
            *this = int2048(-1);
        } else {
            *this = int2048(0);
        }
        return *this;
    }

    std::vector<int> q_digits;
    int2048 remainder(0);
    remainder.digits.clear();

    for (int i = dividend.digits.size() - 1; i >= 0; i--) {
        remainder.digits.insert(remainder.digits.begin(), dividend.digits[i]);
        remainder.remove_leading_zeros();

        int lo = 0, hi = BASE - 1;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            int2048 prod = divisor;
            prod *= int2048((long long)mid);
            if (prod.compare_abs(remainder) <= 0) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }

        q_digits.push_back(lo);
        int2048 sub = divisor;
        sub *= int2048((long long)lo);
        remainder.subtract_abs(sub);
    }

    std::reverse(q_digits.begin(), q_digits.end());
    digits = q_digits;
    remove_leading_zeros();

    bool has_remainder = remainder.digits.size() > 1 || remainder.digits[0] != 0;
    if (result_negative && has_remainder) {
        add_abs(int2048(1));
        negative = true;
    } else {
        negative = result_negative;
    }

    return *this;
}

int2048 &int2048::operator%=(const int2048 &other) {
    int2048 quotient = *this / other;
    quotient *= other;
    *this -= quotient;
    return *this;
}

int2048 operator+(int2048 a, const int2048 &b) {
    return a += b;
}

int2048 operator-(int2048 a, const int2048 &b) {
    return a -= b;
}

int2048 operator*(int2048 a, const int2048 &b) {
    return a *= b;
}

int2048 operator/(int2048 a, const int2048 &b) {
    return a /= b;
}

int2048 operator%(int2048 a, const int2048 &b) {
    return a %= b;
}

std::istream &operator>>(std::istream &is, int2048 &num) {
    std::string s;
    is >> s;
    num.read(s);
    return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &num) {
    if (num.negative) os << '-';
    os << num.digits.back();
    for (int i = num.digits.size() - 2; i >= 0; i--) {
        int val = num.digits[i];
        for (int j = BASE / 10; j > 0; j /= 10) {
            os << (val / j) % 10;
        }
    }
    return os;
}

bool operator==(const int2048 &a, const int2048 &b) {
    return a.negative == b.negative && a.digits == b.digits;
}

bool operator!=(const int2048 &a, const int2048 &b) {
    return !(a == b);
}

bool operator<(const int2048 &a, const int2048 &b) {
    if (a.negative != b.negative) return a.negative;
    if (a.digits.size() != b.digits.size()) {
        return (a.digits.size() < b.digits.size()) != a.negative;
    }
    for (int i = a.digits.size() - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            return (a.digits[i] < b.digits[i]) != a.negative;
        }
    }
    return false;
}

bool operator>(const int2048 &a, const int2048 &b) {
    return b < a;
}

bool operator<=(const int2048 &a, const int2048 &b) {
    return !(a > b);
}

bool operator>=(const int2048 &a, const int2048 &b) {
    return !(a < b);
}

} // namespace sjtu
