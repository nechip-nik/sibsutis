cmake_minimum_required(VERSION 3.10)
project(StreamCipher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wno-unused-function -Wno-aggressive-loop-optimizations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Общая инициализация
add_library(ecrypt_init STATIC ciphers/ecrypt_init.c)

# Библиотеки шифров
add_library(hc128 STATIC ciphers/hc128.c)
add_library(rabbit STATIC ciphers/rabbit.c)
add_library(salsa20 STATIC ciphers/salsa20.c)
add_library(sosemanuk STATIC ciphers/sosemanuk.c)

# Бенчмарк
add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark hc128 rabbit salsa20 sosemanuk ecrypt_init)

# Передача файлов (используем Salsa20)
add_executable(file_transfer src/file_transfer.cpp src/utils.cpp)
target_link_libraries(file_transfer salsa20 ecrypt_init)
