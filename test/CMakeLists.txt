#download gtest configrue and build it
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
#configure
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    RESULT_VARIABLE result)
if(result)
    message(FATAL_ERROR "CMake step for configure google-test failed: ${result}")
endif()

#build
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    RESULT_VARIABLE result)
if(result)
    message(FATAL_ERROR "CMake step for build google-test failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
add_executable(
    unit_tests main.cc test_template_class.cc test_template_function.cc
)

target_link_libraries(
    unit_tests  gtest_main
)

target_include_directories(
    unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/template/include
)