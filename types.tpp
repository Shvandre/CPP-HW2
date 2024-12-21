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

    static constexpr Fixed from_raw(IntType<N> x) {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    IntType<N> v;
    bool operator<=(const Fixed& other) const {
        return v < other.v;
    }
    auto operator<=>(const Fixed& other) const {
        return static_cast<double>(*this) <=> static_cast<double>(other);
    }

    bool operator==(const Fixed&) const = default;

    //Cast from, and to float, double
    explicit operator float() const {
        return v / static_cast<float>(1 << K);
    }
    operator double() const {
        return v / static_cast<double>(1 << K);
    }



    //Оператор деления на int
    Fixed operator/(int x) const {
        return Fixed::from_raw(v / x);
    }
    bool operator<(int x) const {
        return v < (x << K);
    }
    bool operator<=(int x) const {
        return v <= (x << K);
    }
    bool operator>(int x) const {
        return v > (x << K);
    }
    bool operator>=(int x) const {
        return v >= (x << K);
    }
    //Cast from int to Fixed
    Fixed& operator=(int x) {
        v = x << K;
        return *this;
    }

    Fixed operator+(const Fixed& other) const {
        return Fixed::from_raw(v + other.v);
    }

    Fixed operator-(const Fixed& other) const {
        return Fixed::from_raw(v - other.v);
    }

    Fixed operator*(const Fixed& other) const {
        return Fixed::from_raw((static_cast<int64_t>(v) * other.v) >> K);
    }

    Fixed operator/(const Fixed& other) const {
        return Fixed::from_raw((static_cast<int64_t>(v) << K) / other.v);
    }

    Fixed& operator+=(const Fixed& other) {
        v += other.v;
        return *this;
    }

    Fixed& operator-=(const Fixed& other) {
        v -= other.v;
        return *this;
    }

    Fixed& operator*=(const Fixed& other) {
        v = (static_cast<int64_t>(v) * other.v) >> K;
        return *this;
    }

    Fixed& operator/=(const Fixed& other) {
        v = (static_cast<int64_t>(v) << K) / other.v;
        return *this;
    }

    Fixed operator-() const {
        return Fixed::from_raw(-v);
    }

    Fixed abs() const {
        return Fixed::from_raw(v < 0 ? -v : v);
    }

    friend std::ostream& operator<<(std::ostream& out, const Fixed& x) {
        return out << x.v / static_cast<long double>(1 << K);
    }
};
