cmake_minimum_required(VERSION 3.5)

project(gen_bip)

set(CMAKE_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++14")

set(CMAKE_CXX_FLAGS_DEBUG "-g")

add_executable(gen_bip main.cpp MyLib/ParameterReader.cpp MyLib/ParameterReader.h)
