cmake_minimum_required(VERSION 3.16)

project(storage_util_cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(stu
    main.cpp
    args_parser.cpp
)
