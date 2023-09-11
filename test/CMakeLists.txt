 cmake_minimum_required(VERSION 3.1)
 
 add_executable(t tmain.cpp UnitTest.cpp PerformanceTest.cpp)
 target_link_libraries(t PRIVATE gtest)
 
#  add_test(NAME t COMMAND t)
include(GoogleTest)
gtest_add_tests(TARGET t)

 