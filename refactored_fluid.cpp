#include <bits/stdc++.h>

#include "FluidCalc.tpp"
#include "types.tpp"



constexpr size_t N = 36, M = 84;
constexpr size_t T = 1'000'000;
char field[N][M + 1] = {
    "####################################################################################",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                       .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............################                     #                 #",
    "#...........................#....................................#                 #",
    "#...........................#....................................#                 #",
    "#...........................#....................................#                 #",
    "##################################################################                 #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "####################################################################################",
};
using Datatype = Fixed<Int, 32, 16>;
//using Fluid = FluidCalc<N, M, Datatype, true>;
#define S(a, b) {{a, b}, createFluid<a, b, TYPES>()}

#define TYPES FAST_FIXED(13,7),FIXED(32,5),DOUBLE
#define SIZES S(1920,1080),S(10,10),S(42,1337)

#define FLOAT float
#define FAST_FIXED(width, precision) Fixed<FastInt, width, precision>
#define FIXED(width, precision) Fixed<Int, width, precision>
#define DOUBLE double


class fixed {
public:
    static std::string name() {
        return "fixed";
    }
};

class dynamic {
public:
    static std::string name() {
        return "dynamic";
    }
};

template<typename A, typename B>
struct test {
    A x;
    B y;
};

template<typename First, typename ...Args>
std::any get_test(const std::string &typeA, const std::string &typeB) {
    if (First::name() == typeA){
        return test<First, Args...>();
    }
    else if constexpr (sizeof...(Args) > 0) {
        return get_test<Args...>(typeA, typeB);
    }
    else {
        throw std::runtime_error("No such type");
    }
}


int main() {
    // Создаем Fluid для разных типов и размеров
    //std::map<std::pair<int, int>, std::vector<std::any>> v = {SIZES};
    FluidCalc<N, M, Fixed<Int, 32, 16>, Fixed<Int, 32, 16>, Fixed<FastInt, 32, 16>, false> v;
    v.run(1000);

    return 0;
    int a, b;
    std::cin >> a >> b;

    //if (!v.contains({a, b})) {
    //    std::cout << "Unsupported size for static calculations." << std::endl;
    //    return 1; //TODO dynamic calculations
    //}
    int x;
    std::cin >> x;

    //std::shared_ptr<std::any> my_fluid = v[std::make_pair(a, b)][0];

    return 0;
}