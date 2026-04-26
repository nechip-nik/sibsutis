#include "sha3.h"
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

static const uint64_t RC[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

static const int RHO[24] = {
    1, 62, 28, 27, 36, 44, 6, 55, 20, 3, 10, 43, 25, 39, 41, 45, 15, 21, 8, 18, 2, 61, 56, 14
};

static const int PI[24] = {
    10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6, 1
};

uint64_t SHA3::rotl64(uint64_t x, int n) {
    return (x << n) | (x >> (64 - n));
}

void SHA3::keccak_f(uint64_t state[25]) {
    uint64_t bc[5], t;
    
    for (int round = 0; round < 24; round++) {
        // Theta
        for (int i = 0; i < 5; i++) {
            bc[i] = state[i] ^ state[i + 5] ^ state[i + 10] ^ state[i + 15] ^ state[i + 20];
        }
        for (int i = 0; i < 5; i++) {
            t = bc[(i + 4) % 5] ^ rotl64(bc[(i + 1) % 5], 1);
            for (int j = 0; j < 25; j += 5) {
                state[j + i] ^= t;
            }
        }
        
        // Rho and Pi
        t = state[1];
        for (int i = 0; i < 24; i++) {
            int j = PI[i];
            uint64_t tmp = rotl64(t, RHO[i]);
            t = state[j];
            state[j] = tmp;
        }
        
        // Chi
        for (int j = 0; j < 25; j += 5) {
            for (int i = 0; i < 5; i++) bc[i] = state[j + i];
            for (int i = 0; i < 5; i++) state[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
        }
        
        // Iota
        state[0] ^= RC[round];
    }
}

void SHA3::sha3_hash(const uint8_t* input, size_t len, uint8_t* output, size_t output_bits) {
    size_t rate = 1600 - 2 * output_bits;
    size_t rate_bytes = rate / 8;
    size_t output_bytes = output_bits / 8;
    
    vector<uint64_t> state(25, 0);
    
    // Absorbing
    vector<uint8_t> block(rate_bytes, 0);
    size_t offset = 0;
    
    while (len >= rate_bytes) {
        for (size_t i = 0; i < rate_bytes; i++) {
            block[i] = input[offset + i];
        }
        for (size_t i = 0; i < rate_bytes / 8; i++) {
            uint64_t val = 0;
            for (int j = 0; j < 8; j++) {
                val |= (uint64_t)block[i * 8 + j] << (j * 8);
            }
            state[i] ^= val;
        }
        keccak_f(state.data());
        offset += rate_bytes;
        len -= rate_bytes;
    }
    
    // Last block
    for (size_t i = 0; i < rate_bytes; i++) block[i] = 0;
    for (size_t i = 0; i < len; i++) block[i] = input[offset + i];
    block[len] ^= 0x06;
    block[rate_bytes - 1] ^= 0x80;
    
    for (size_t i = 0; i < rate_bytes / 8; i++) {
        uint64_t val = 0;
        for (int j = 0; j < 8; j++) {
            val |= (uint64_t)block[i * 8 + j] << (j * 8);
        }
        state[i] ^= val;
    }
    keccak_f(state.data());
    
    // Squeezing
    size_t out_offset = 0;
    while (output_bytes > 0) {
        size_t copy = min(output_bytes, rate_bytes);
        for (size_t i = 0; i < copy / 8; i++) {
            uint64_t val = state[i];
            for (int j = 0; j < 8 && out_offset + i * 8 + j < output_bytes; j++) {
                output[out_offset + i * 8 + j] = (val >> (j * 8)) & 0xFF;
            }
        }
        out_offset += copy;
        output_bytes -= copy;
        if (output_bytes > 0) {
            keccak_f(state.data());
        }
    }
}

vector<uint8_t> SHA3::hash256(const vector<uint8_t>& data) {
    return hash256(data.data(), data.size());
}

vector<uint8_t> SHA3::hash256(const uint8_t* data, size_t len) {
    vector<uint8_t> result(32);
    sha3_hash(data, len, result.data(), 256);
    return result;
}

string SHA3::hash256_hex(const string& data) {
    auto hash = hash256((const uint8_t*)data.c_str(), data.size());
    char hex[65];
    for (int i = 0; i < 32; i++) {
        sprintf(hex + i * 2, "%02x", hash[i]);
    }
    return string(hex);
}

vector<uint8_t> SHA3::hash512(const vector<uint8_t>& data) {
    return hash512(data.data(), data.size());
}

vector<uint8_t> SHA3::hash512(const uint8_t* data, size_t len) {
    vector<uint8_t> result(64);
    sha3_hash(data, len, result.data(), 512);
    return result;
}

string SHA3::hash512_hex(const string& data) {
    auto hash = hash512((const uint8_t*)data.c_str(), data.size());
    char hex[129];
    for (int i = 0; i < 64; i++) {
        sprintf(hex + i * 2, "%02x", hash[i]);
    }
    return string(hex);
}

vector<uint8_t> SHA3::hmac256(const vector<uint8_t>& key, const vector<uint8_t>& data) {
    const size_t block_size = 136;
    vector<uint8_t> padded_key(block_size, 0);
    
    if (key.size() > block_size) {
        auto key_hash = hash256(key);
        copy(key_hash.begin(), key_hash.end(), padded_key.begin());
    } else {
        copy(key.begin(), key.end(), padded_key.begin());
    }
    
    vector<uint8_t> ipad(block_size), opad(block_size);
    for (size_t i = 0; i < block_size; i++) {
        ipad[i] = padded_key[i] ^ 0x36;
        opad[i] = padded_key[i] ^ 0x5c;
    }
    
    vector<uint8_t> inner_data;
    inner_data.insert(inner_data.end(), ipad.begin(), ipad.end());
    inner_data.insert(inner_data.end(), data.begin(), data.end());
    auto inner_hash = hash256(inner_data);
    
    vector<uint8_t> outer_data;
    outer_data.insert(outer_data.end(), opad.begin(), opad.end());
    outer_data.insert(outer_data.end(), inner_hash.begin(), inner_hash.end());
    
    return hash256(outer_data);
}

vector<uint8_t> SHA3::hmac512(const vector<uint8_t>& key, const vector<uint8_t>& data) {
    const size_t block_size = 72;
    vector<uint8_t> padded_key(block_size, 0);
    
    if (key.size() > block_size) {
        auto key_hash = hash512(key);
        copy(key_hash.begin(), key_hash.end(), padded_key.begin());
    } else {
        copy(key.begin(), key.end(), padded_key.begin());
    }
    
    vector<uint8_t> ipad(block_size), opad(block_size);
    for (size_t i = 0; i < block_size; i++) {
        ipad[i] = padded_key[i] ^ 0x36;
        opad[i] = padded_key[i] ^ 0x5c;
    }
    
    vector<uint8_t> inner_data;
    inner_data.insert(inner_data.end(), ipad.begin(), ipad.end());
    inner_data.insert(inner_data.end(), data.begin(), data.end());
    auto inner_hash = hash512(inner_data);
    
    vector<uint8_t> outer_data;
    outer_data.insert(outer_data.end(), opad.begin(), opad.end());
    outer_data.insert(outer_data.end(), inner_hash.begin(), inner_hash.end());
    
    return hash512(outer_data);
}
