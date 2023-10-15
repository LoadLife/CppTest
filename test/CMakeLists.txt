file(GLOB TEST_SRC *.cc)
add_executable(
    unit_tests ${TEST_SRC})
target_link_libraries(unit_tests struct gtest gtest_main)
target_include_directories(unit_tests PUBLIC ${CMAKE_SOURCE_DIR}/struct/include)

add_dependencies(unit_tests struct)