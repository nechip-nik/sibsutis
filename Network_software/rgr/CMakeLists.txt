cmake_minimum_required(VERSION 3.20)
project(chess_net LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.74 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(nlohmann_json 3.10 REQUIRED)
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/third_party
)

add_executable(game_server
    server/main.cpp
    server/game_server.cpp
    server/chat_server.cpp
)
target_include_directories(game_server PRIVATE ${COMMON_INCLUDES} server)
target_link_libraries(game_server PRIVATE
    Boost::headers
    SQLite::SQLite3
    nlohmann_json::nlohmann_json
)

add_executable(chess_client client/main.cpp)
target_include_directories(chess_client PRIVATE ${COMMON_INCLUDES} client)
target_link_libraries(chess_client PRIVATE
    Boost::headers
    nlohmann_json::nlohmann_json
    SFML::Graphics SFML::Window SFML::System
)