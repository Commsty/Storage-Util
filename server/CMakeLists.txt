cmake_minimum_required(VERSION 3.10)
project(StuServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(stu_server
    main.cpp
    service/storage_service.cpp
    data_access/data_access.cpp 
)

if(UNIX)
    target_link_libraries(stu_server pthread)
endif()