 cmake_minimum_required(VERSION 3.1)
 
 add_executable(t tmain.cpp UnitTest.cpp PerformanceTest.cpp)
 target_link_libraries(t PRIVATE gtest)

#link b+tree.cpp
 target_link_libraries(t PRIVATE bplustree)
 target_include_directories(t PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

#  add_test(NAME t COMMAND t)
include(GoogleTest)
gtest_add_tests(TARGET t)

#boost
# 查找Boost库并添加链接
find_package(Boost REQUIRED COMPONENTS serialization)
include_directories(${Boost_INCLUDE_DIRS})
# 链接Boost库
target_link_libraries(t PRIVATE ${Boost_LIBRARIES})

 