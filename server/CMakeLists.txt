cmake_minimum_required(VERSION 3.10)
project(StuServer)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(stu_server
    main.cpp
    service/storage_service.cpp
    data_access/data_access.cpp 
    data_access/json_data_access.cpp
)

target_link_libraries(stu_server PRIVATE nlohmann_json::nlohmann_json)

if(UNIX)
    target_link_libraries(stu_server pthread)
endif()