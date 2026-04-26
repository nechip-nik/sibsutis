#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cstdint>

typedef uint8_t u8;
typedef uint32_t u32;

std::string bytes_to_hex(const uint8_t* bytes, size_t len);
std::vector<uint8_t> hex_to_bytes(const std::string& hex);
void print_progress(double percentage);

#endif