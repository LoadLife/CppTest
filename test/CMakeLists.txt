#download gtest configrue and build it
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
#configure
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    RESULT_VARIABLE result)
if(result)
    message(FATAL_ERROR "CMake step for configure google-test failed: ${result}")
endif()

#build-download(the time to real download)
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
    RESULT_VARIABLE result)
if(result)
    message(FATAL_ERROR "CMake step for build google-test failed: ${result}")
endif()

#add sub google_test dir
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

#generate executable
file(GLOB TEST_SRC *.cc)
add_executable(
    unit_tests ${TEST_SRC} ${TEMPLATE_SRC}
)
target_link_libraries(
    unit_tests  gtest_main
)
target_include_directories(
    unit_tests PUBLIC ${CMAKE_SOURCE_DIR}/template/include
)