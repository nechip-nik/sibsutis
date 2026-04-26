cmake_minimum_required(VERSION 3.10)
project(ECMQV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем предупреждения о deprecated функциях
add_definitions(-Wno-deprecated-declarations)

find_package(OpenSSL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(ecmqv
    src/main.cpp
)

target_link_libraries(ecmqv ${OPENSSL_LIBRARIES} crypto ssl)