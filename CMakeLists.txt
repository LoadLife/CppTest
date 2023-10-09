cmake_minimum_required(VERSION 3.17)
project(Template_Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(third_part/googletest)
add_subdirectory(test)
add_subdirectory(struct)