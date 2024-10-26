cmake_minimum_required(VERSION 3.28)
project(game2048)



set(CMAKE_CXX_STANDARD 26)
link_libraries(libeasyx.a)
add_executable(game2048 main.cpp
        allhe.h)
target_link_libraries(game2048 PRIVATE easyx msimg32)