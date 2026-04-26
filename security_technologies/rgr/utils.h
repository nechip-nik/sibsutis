#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <gmp.h>

inline std::string bytesToHex(const uint8_t* bytes, size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        oss << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return oss.str();
}

inline std::vector<uint8_t> mpzToVector(const mpz_t num) {
    std::vector<uint8_t> bytes(32, 0);
    size_t count;
    mpz_export(bytes.data(), &count, 1, 1, 0, 0, num);
    if (count < 32) {
        memmove(bytes.data() + (32 - count), bytes.data(), count);
        memset(bytes.data(), 0, 32 - count);
    }
    return bytes;
}

#endif