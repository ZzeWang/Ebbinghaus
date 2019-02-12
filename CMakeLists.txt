cmake_minimum_required(VERSION 3.9)
project(AAebb)

set(CMAKE_CXX_STANDARD 11)

add_executable(AAebb src/main.cpp src/IO.cpp src/Project.cpp src/symbol.cpp src/cmdt.cpp include/cflag.h src/cflag.cpp)

