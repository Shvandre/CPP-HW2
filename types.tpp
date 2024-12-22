#pragma once
#include <bits/stdc++.h>

template<int N>
struct IntSelector {
    using type = std::conditional_t<
        (N == 8), int8_t,
        std::conditional_t<
            (N == 16), int16_t,
            std::conditional_t<
                (N == 32), int32_t,
                std::conditional_t<
                    (N == 64), int64_t,
                    void
                >
            >
        >
    >;
    static_assert(!std::is_same_v<type, void>, "Unsupported integer size. Supported sizes are 8, 16, 32, 64 bits.");
};
template<int N>
using Int = typename IntSelector<N>::type;

template<int N>
struct FastIntSelector {
    using type = std::conditional_t<
        (N > 0 && N <= 8), int_fast8_t,
        std::conditional_t<
            (N <= 16), int_fast16_t,
            std::conditional_t<
                (N <= 32), int_fast32_t,
                std::conditional_t<
                    (N <= 64), int_fast64_t,
                    void
                >
            >
        >
    >;
    static_assert(!std::is_same_v<type, void>, "Unsupported integer size. Supported sizes are from 1 to 64 bits.");
};
template<int N>
using FastInt = typename FastIntSelector<N>::type;


template <template<int> class IntType, int N, int K>
struct Fixed {
    constexpr Fixed(int v): v(v << K) {}
    constexpr Fixed(float f): v(f * (1 << K)) {}
    constexpr Fixed(double f): v(f * (1 << K)) {}
    constexpr Fixed(Fixed&& other) {
        Fixed(static_cast<double>(other));
    }
    constexpr Fixed(const Fixed& other) {
        Fixed(static_cast<double>(other));
    }
    constexpr Fixed(): v(0) {}

    static constexpr Fixed max() {
        return Fixed::from_raw(std::numeric_limits<IntType<N>>::max());
    }
    Fixed& operator=(int x) {
        v = x << K;
        return *this;
    }

    static constexpr Fixed from_raw(IntType<N> x) {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    IntType<N> v;

    explicit operator float() const {
        return v / static_cast<float>(1 << K);
    }
    operator double() const {
        return v / static_cast<double>(1 << K);
    }

    Fixed& operator+=(double x) {
        v += Fixed(x).v;
        return *this;
    }

    template <template<int> class OtherIntType>
    operator Fixed<OtherIntType, N, K>() const {
        Fixed<OtherIntType, N, K> result;
        result.v = static_cast<OtherIntType<N>>(v);
        return result;
    }
    constexpr Fixed& operator=(const Fixed& other) noexcept {
        if (this != &other) {
            v = other.v;
        }
        return *this;
    }
    Fixed abs() const {
        return Fixed::from_raw(v < 0 ? -v : v);
    }

    friend std::ostream& operator<<(std::ostream& out, const Fixed& x) {
        return out << x.v / static_cast<long double>(1 << K);
    }
};

// Comparison operators
template <template<int> class IntType, int N, int K>
bool operator<=(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return lhs.v < rhs.v;
}

template <template<int> class IntType, int N, int K>
auto operator<=>(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return static_cast<double>(lhs) <=> static_cast<double>(rhs);
}

template <template<int> class IntType, int N, int K>
bool operator==(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return lhs.v == rhs.v;
}

// Arithmetic operators
template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator/(const Fixed<IntType, N, K>& lhs, int x) {
    return Fixed<IntType, N, K>::from_raw(lhs.v / x);
}

template <template<int> class IntType, int N, int K>
bool operator<(const Fixed<IntType, N, K>& lhs, int x) {
    return lhs.v < (x << K);
}

template <template<int> class IntType, int N, int K>
bool operator<=(const Fixed<IntType, N, K>& lhs, int x) {
    return lhs.v <= (x << K);
}

template <template<int> class IntType, int N, int K>
bool operator>(const Fixed<IntType, N, K>& lhs, int x) {
    return lhs.v > (x << K);
}

template <template<int> class IntType, int N, int K>
bool operator>=(const Fixed<IntType, N, K>& lhs, int x) {
    return lhs.v >= (x << K);
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator+(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return Fixed<IntType, N, K>::from_raw(lhs.v + rhs.v);
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator-(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return Fixed<IntType, N, K>::from_raw(lhs.v - rhs.v);
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator*(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return Fixed<IntType, N, K>::from_raw((static_cast<int64_t>(lhs.v) * rhs.v) >> K);
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator/(const Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    return Fixed<IntType, N, K>::from_raw((static_cast<int64_t>(lhs.v) << K) / rhs.v);
}

// Compound assignment operators
template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K>& operator+=(Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    if((long long)lhs.v + (long long)rhs.v > std::numeric_limits<IntType<N>>::max()) {
        lhs.v = std::numeric_limits<IntType<N>>::max();
    } else {
        lhs.v += rhs.v;
    }
    lhs.v += rhs.v;
    return lhs;
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K>& operator-=(Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    lhs.v -= rhs.v;
    return lhs;
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K>& operator*=(Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    lhs.v = (static_cast<int64_t>(lhs.v) * rhs.v) >> K;
    return lhs;
}

template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K>& operator/=(Fixed<IntType, N, K>& lhs, const Fixed<IntType, N, K>& rhs) {
    lhs.v = (static_cast<int64_t>(lhs.v) << K) / rhs.v;
    return lhs;
}

// Unary operators
template <template<int> class IntType, int N, int K>
Fixed<IntType, N, K> operator-(const Fixed<IntType, N, K>& x) {
    return Fixed<IntType, N, K>::from_raw(-x.v);
}