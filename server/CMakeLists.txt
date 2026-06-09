cmake_minimum_required(VERSION 3.16)

project(ChatServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(union_server
    src/main.cpp
    src/Server.cpp
)

target_link_libraries(union_server
    ws2_32
)