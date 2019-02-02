cmake_minimum_required(VERSION 3.9)
project(AAebb)

set(CMAKE_CXX_STANDARD 11)

add_executable(AAebb src/main.cpp src/IO.cpp src/Project.cpp src/symbol.cpp src/cmdt.cpp)
# add_executable(AAebb main.cpp Project.h Job.h IO.h IO.cpp Project.cpp symbol.cpp cmd.h cmdt.cpp)
#add_executable(AAebb main.cpp cmdt.cpp)
#441a300