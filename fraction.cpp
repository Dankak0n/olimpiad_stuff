template <typename T = long long, bool IS_RECALC = true>
struct frac {
    T num;
    T den;
    inline void relax() {
        if (den < 0) {
            num = -num;
            den = -den;
        }
        if (IS_RECALC) {
            T _gcd = std::__gcd(num < 0 ? -num : num, den);
            num /= _gcd;
            den /= _gcd;
        }
    }
    explicit frac(const T& x, const T& y) : num{x}, den{y} {
        relax();
    }
    frac() : frac{0, 1} {
    }
    frac(const T& x) : frac{x, 1} {
    }
    frac(const std::pair<T, T>& x) : frac{x.first, x.second} {
    }
    frac(const frac& other) : frac{other.num, other.den} {
    }
    explicit operator long double() const {
        return 1.L * num / den;
    }
    explicit operator std::pair<T, T>() const {
        return std::pair<T, T>{num, den};
    }
    explicit operator bool() const {
        return !(num == (T)0);
    }
    frac& operator+=(const T& other) {
        num += other * den;
        relax();
        return *this;
    }
    frac& operator-=(const T& other) {
        num -= other * den;
        relax();
        return *this;
    }
    frac& operator*=(const T& other) {
        num *= other;
        relax();
        return *this;
    }
    frac& operator/=(const T& other) {
        den *= other;
        relax();
        return *this;
    }
    friend frac operator+(frac lhs, const frac& rhs) {
        return lhs += rhs;
    }
    friend frac operator-(frac lhs, const frac& rhs) {
        return lhs -= rhs;
    }
    friend frac operator*(frac lhs, const frac& rhs) {
        return lhs *= rhs;
    }
    friend frac operator/(frac lhs, const frac& rhs) {
        return lhs /= rhs;
    }
    friend frac operator-(const frac &x) {
        return frac {-x.num, x.den};
    }
    friend bool operator==(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den == rhs.num * lhs.den;
    }
    friend bool operator!=(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den != rhs.num * lhs.den;
    }
    friend bool operator<(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den < rhs.num * lhs.den;
    }
    friend bool operator>(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den > rhs.num * lhs.den;
    }
    friend bool operator<=(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den <= rhs.num * lhs.den;
    }
    friend bool operator>=(const frac& lhs, const frac& rhs) {
        return lhs.num * rhs.den >= rhs.num * lhs.den;
    }
};
