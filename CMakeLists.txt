cmake_minimum_required(VERSION 3.10)
project(Template_Test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
file(GLOB TEMPLATE_SRC template/src/*.cc)
add_subdirectory(test)