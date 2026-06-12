# Sets the minimum required version of CMake for the build system
cmake_minimum_required(VERSION 3.15)

# Declares the project name and programming language
project(UnionServer LANGUAGES CXX)

# Forces compliance with the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiles the executable "MyExecutable" from the source file "main.cpp"
add_executable(union_client src/main.cpp
    include/Client.h
    src/Client.cpp)


# Link the Winsock2 library ONLY on Windows systems
if(WIN32)
    target_link_libraries(union_client PRIVATE ws2_32)
endif()