cmake_minimum_required(VERSION 3.16)

project(ChatServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###
#   Testing library
###
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# Prevent overriding the parent project's compiler settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


###
#   Executables
###

add_library(my_library
    src/Server.cpp
)

add_executable(union_server
    src/main.cpp
    src/Server.cpp
)


target_link_libraries(union_server
    ws2_32
)

enable_testing()

add_subdirectory(tests)


