file(GLOB TEST_SRC *.cc)
add_executable(
    unit_tests ${TEST_SRC})

target_link_libraries(
    unit_tests gtest gtest_main struct)

target_include_directories(
    unit_tests PUBLIC ${CMAKE_SOURCE_DIR}/struct_define/include)