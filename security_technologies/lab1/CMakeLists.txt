cmake_minimum_required(VERSION 3.10)
project(DiffieHellmanProject)

set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_SOURCE_DIR}/include)

# 1. Генерация параметров
add_executable(generate_params src/main_generate_params.cpp src/crypto_utils.cpp)
target_link_libraries(generate_params gmp)

# 2. Оригинальный DH
add_executable(dh_original src/main_dh_original.cpp src/crypto_utils.cpp)
target_link_libraries(dh_original gmp)

# 3. DH в подгруппе
add_executable(dh_subgroup src/main_dh_subgroup.cpp src/crypto_utils.cpp)
target_link_libraries(dh_subgroup gmp)

# 4. MQV
add_executable(mqv src/main_mqv.cpp src/crypto_utils.cpp)
target_link_libraries(mqv gmp)

# 5. Сетевая часть
add_executable(server src/network_server.cpp src/crypto_utils.cpp)
target_link_libraries(server gmp)

add_executable(client src/network_client.cpp src/crypto_utils.cpp)
target_link_libraries(client gmp)

# 6. Упрощенный бенчмарк
add_executable(benchmark_simple src/benchmark_simple.cpp src/crypto_utils.cpp)
target_link_libraries(benchmark_simple gmp)