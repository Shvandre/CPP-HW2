cmake_minimum_required(VERSION 3.29)
project(fluid)

set(CMAKE_CXX_STANDARD 20)

# Add sanitizers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=pointer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=alignment -fsanitize=object-size -fsanitize=integer-divide-by-zero -fsanitize=float-divide-by-zero -fsanitize=return -fsanitize=vla-bound -fsanitize=vptr -fsanitize=enum -fsanitize=bool -fsanitize=undefined -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=unreachable -fsanitize=return -fsanitize=integer-divide-by-zero -fsanitize=float-divide-by-zero -fsanitize=shift -fsanitize=unreachable")
#add_executable(fluid refactored_fluid.cpp types.tpp FluidCalc.tpp)
add_executable(fluid types.tpp FluidCalc.tpp
        TypesTesting.cpp)
#add_executable(fluid afluid.cpp)
#add_executable(fluid EFluid.cpp main.cpp ETypes.hpp)

