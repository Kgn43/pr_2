cmake_minimum_required(VERSION 3.25)
project(pr_2)
project(client)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(pr_2
        server.cpp
        makeStructure.cpp
        arr.cpp
        insert.cpp
        common.cpp
        delete.cpp
        select.cpp
        userQuery.cpp)

add_executable(client client.cpp)

target_link_libraries(pr_2 PRIVATE nlohmann_json::nlohmann_json)

