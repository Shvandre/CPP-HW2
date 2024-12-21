cmake_minimum_required(VERSION 3.29)
project(fluid)

set(CMAKE_CXX_STANDARD 20)

add_executable(fluid refactored_fluid.cpp types.tpp FluidCalc.tpp)
#add_executable(fluid afluid.cpp)
#add_executable(fluid EFluid.cpp main.cpp ETypes.hpp)

