#include "types.tpp"
#include <iostream>
#include <cassert>
#include <cmath>

const double eps = 1e-2;

int main() {
    // Test 1: Basic arithmetic with Fixed<Int, 32, 8>
    {
        Fixed<Int, 32, 8> a(5.5);
        Fixed<Int, 32, 8> b(2.25);
        Fixed<Int, 32, 8> c = a + b;
        assert(std::fabs(static_cast<double>(c) - 7.75) < eps);

        c = a - b;
        assert(std::fabs(static_cast<double>(c) - 3.25) < eps);

        c = a * b;
        assert(std::fabs(static_cast<double>(c) - 12.375) < eps);

        c = a / b;
        std::cout << c << std::endl;
        assert(std::fabs(static_cast<double>(c) - 2.444444) < eps);

        std::cout << "Test 1 passed\n";
    }

    // Test 2: Comparisons with Fixed<Int, 32, 8>
    {
        Fixed<Int, 32, 8> a(3.5);
        Fixed<Int, 32, 8> b(3.5);
        Fixed<Int, 32, 8> c(2.75);

        assert(a == b);
        assert(a != c);
        assert(a > c);
        assert(c < a);
        assert(a >= b);
        assert(c <= a);

        std::cout << "Test 2 passed\n";
    }

    // Test 3: Mixed arithmetic with Fixed<Int, 32, 8> and doubles
    {
        Fixed<Int, 32, 8> a(4.75);
        double b = 3.25;

        Fixed<Int, 32, 8> c = a + Fixed<Int, 32, 8>(b);
        assert(std::fabs(static_cast<double>(c) - 8.0) < eps);

        c = a - Fixed<Int, 32, 8>(b);
        assert(std::fabs(static_cast<double>(c) - 1.5) < eps);

        c = a * Fixed<Int, 32, 8>(b);
        assert(std::fabs(static_cast<double>(c) - 15.4375) < eps);

        c = a / Fixed<Int, 32, 8>(b);
        assert(std::fabs(static_cast<double>(c) - 1.461538) < eps);

        std::cout << "Test 3 passed\n";
    }

    // Test 4: Fixed<FastInt, 16, 4> with different scales
    {
        const double newEps = 1e-1;
        Fixed<FastInt, 16, 4> a(1.125);
        Fixed<FastInt, 16, 4> b(0.875);
        Fixed<FastInt, 16, 4> c = a + b;

        assert(std::fabs(static_cast<double>(c) - 2.0) < newEps);

        c = a - b;
        assert(std::fabs(static_cast<double>(c) - 0.25) < newEps);

        c = a * b;
        std::cout << c << std::endl;
        assert(std::fabs(static_cast<double>(c) - 0.984375) < newEps);

        c = a / b;
        std::cout << c << std::endl;
        assert(std::fabs(static_cast<double>(c) - 1.285714) < newEps);

        std::cout << "Test 4 passed\n";
    }

    // Test 5: Conversion between Fixed types
    {
        Fixed<Int, 32, 8> a(7.5);
        Fixed<FastInt, 16, 4> b = static_cast<Fixed<FastInt, 16, 4>>(a);
        assert(std::fabs(static_cast<double>(b) - 7.5) < eps);

        std::cout << "Test 5 passed\n";
    }

    std::cout << "All tests passed successfully!\n";
    return 0;
}
