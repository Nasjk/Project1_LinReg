cmake_minimum_required(VERSION 3.20)
project(lin_reg_test_cpp)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(run_lin_reg_test_cpp ../lin_reg_test.cpp ../lin_reg.cpp)
target_compile_options(run_lin_reg_test_cpp PRIVATE -Wall -Werror)
target_link_libraries(run_lin_reg_test_cpp pthread ${GTEST_LIBRARIES})
set_target_properties(run_lin_reg_test_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)