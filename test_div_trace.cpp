#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

namespace sjtu {

static const int BASE = 1000000000;
static const int BASE_DIGITS = 9;

class int2048 {
private:
    std::vector<int> digits;
    bool negative;

    void remove_leading_zeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0) {
            negative = false;
        }
    }

    int compare_abs(const int2048 &other) const {
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

    void add_abs(const int2048 &other) {
        int carry = 0;
        for (size_t i = 0; i < std::max(digits.size(), other.digits.size()) || carry; i++) {
            if (i == digits.size()) digits.push_back(0);
            digits[i] += carry + (i < other.digits.size() ? other.digits[i] : 0);
            carry = digits[i] >= BASE;
            if (carry) digits[i] -= BASE;
        }
    }

    void subtract_abs(const int2048 &other) {
        int carry = 0;
        for (size_t i = 0; i < other.digits.size() || carry; i++) {
            digits[i] -= carry + (i < other.digits.size() ? other.digits[i] : 0);
            carry = digits[i] < 0;
            if (carry) digits[i] += BASE;
        }
        remove_leading_zeros();
    }

public:
    int2048() : negative(false) {
        digits.push_back(0);
    }

    int2048(long long num) : negative(num < 0) {
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

    int2048(const std::string &s) {
        read(s);
    }

    void read(const std::string &s) {
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

    int2048 &operator*=(const int2048 &other) {
        bool result_negative = negative != other.negative;

        std::vector<int> result(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); i++) {
            long long carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; j++) {
                long long cur = result[i + j] +
                               (long long)digits[i] * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                result[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }

        digits = result;
        negative = result_negative;
        remove_leading_zeros();
        return *this;
    }

    int2048 &operator/=(const int2048 &other) {
        bool dividend_negative = negative;
        bool divisor_negative = other.negative;
        bool result_negative = dividend_negative != divisor_negative;

        std::cout << "DEBUG: dividend_negative=" << dividend_negative
                  << " divisor_negative=" << divisor_negative
                  << " result_negative=" << result_negative << std::endl;

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

        std::cout << "DEBUG: quotient (abs) = " << digits[0] << std::endl;
        std::cout << "DEBUG: remainder = " << remainder.digits[0] << std::endl;

        bool has_remainder = remainder.digits.size() > 1 || remainder.digits[0] != 0;
        std::cout << "DEBUG: has_remainder = " << has_remainder << std::endl;

        if (result_negative && has_remainder) {
            std::cout << "DEBUG: Subtracting 1 from quotient" << std::endl;
            std::cout << "DEBUG: Before subtract: " << digits[0] << std::endl;
            subtract_abs(int2048(1));
            std::cout << "DEBUG: After subtract: " << digits[0] << std::endl;
            negative = true;
        } else {
            negative = result_negative;
        }

        std::cout << "DEBUG: Final quotient = " << (negative ? "-" : "") << digits[0] << std::endl;

        return *this;
    }

    friend int2048 operator/(int2048 a, const int2048 &b) {
        return a /= b;
    }

    friend std::ostream &operator<<(std::ostream &os, const int2048 &num) {
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
};

} // namespace sjtu

int main() {
    sjtu::int2048 a("-10");
    sjtu::int2048 b("3");

    std::cout << "Computing -10 / 3:" << std::endl;
    sjtu::int2048 q = a / b;
    std::cout << "Result: " << q << std::endl;
    std::cout << "Expected: -4" << std::endl;

    return 0;
}
