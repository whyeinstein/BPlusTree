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