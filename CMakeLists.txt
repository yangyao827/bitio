cmake_minimum_required(VERSION 3.21)
project(bitTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(bitTest main.cpp bitio.cpp bitio.h)
