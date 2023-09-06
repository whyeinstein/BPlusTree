cmake_minimum_required(VERSION 3.1)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(myTest test.cpp)
target_include_directories(myTest PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(myTest ${GTEST_LIBRARIES} pthread)