#add_library(hello STATIC Hello.cpp)

#target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/include)

#add_executable(main Main.cpp)

#target_link_libraries(main PRIVATE hello)

#target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)



add_library(bplustree STATIC B+tree.cpp)

target_include_directories(bplustree PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(main TreeMain.cpp)

target_link_libraries(main PRIVATE bplustree)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_library(compare STATIC Comparison.cpp)

target_include_directories(compare PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(main PRIVATE compare)


# 查找Boost库并添加链接
find_package(Boost REQUIRED COMPONENTS serialization)


include_directories(${Boost_INCLUDE_DIRS})

# 链接Boost库
target_link_libraries(main PRIVATE ${Boost_LIBRARIES})