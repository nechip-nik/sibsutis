#ifndef SHA3_H
#define SHA3_H

#include <string>
#include <vector>
#include <cstdint>

class SHA3 {
public:
    // SHA3-256
    static std::vector<uint8_t> hash256(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> hash256(const uint8_t* data, size_t len);
    static std::string hash256_hex(const std::string& data);
    
    // SHA3-512
    static std::vector<uint8_t> hash512(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> hash512(const uint8_t* data, size_t len);
    static std::string hash512_hex(const std::string& data);
    
    // HMAC-SHA3
    static std::vector<uint8_t> hmac256(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
    static std::vector<uint8_t> hmac512(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
    
private:
    static void keccak_f(uint64_t state[25]);
    static void sha3_hash(const uint8_t* input, size_t len, uint8_t* output, size_t output_bits);
    static uint64_t rotl64(uint64_t x, int n);
};

#endif
