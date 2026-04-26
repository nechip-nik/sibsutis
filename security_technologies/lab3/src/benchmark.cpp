#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <random>
#include <cstring>
#include <algorithm>

extern "C" {
#include "../ciphers/hc128.h"
#include "../ciphers/rabbit.h"
#include "../ciphers/salsa20.h"
#include "../ciphers/sosemanuk.h"
}

using namespace std;

class Timer {
private:
    chrono::high_resolution_clock::time_point start_time;
public:
    void start() { start_time = chrono::high_resolution_clock::now(); }
    double stop() {
        auto end_time = chrono::high_resolution_clock::now();
        return chrono::duration<double>(end_time - start_time).count();
    }
};

void random_data(u8* data, size_t size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < size; i++) data[i] = dis(gen);
}

struct Result {
    string name;
    double key_setup_us;
    double iv_setup_us;
    double speeds[5];
    double mbps;
};

int main() {
    cout << "========================================" << endl;
    cout << "Stream Cipher Benchmark" << endl;
    cout << "========================================" << endl;
    cout << "Ciphers: HC-128, Rabbit, Salsa20, Sosemanuk" << endl;
    cout << "Key: 256 bits, IV: 128 bits" << endl;
    cout << "========================================\n" << endl;
    
    ECRYPT_init();
    
    vector<size_t> sizes = {1024, 10240, 102400, 1048576, 10485760};
    vector<u8> plain(sizes.back()), cipher(sizes.back());
    random_data(plain.data(), sizes.back());
    
    u8 key[32], iv[16];
    random_data(key, 32);
    random_data(iv, 16);
    
    Timer timer;
    vector<Result> results(4);
    
    // HC-128
    {
        HC128_ctx ctx;
        results[0].name = "HC-128";
        timer.start(); hc128_keysetup(&ctx, key, 256, 128); results[0].key_setup_us = timer.stop() * 1e6;
        timer.start(); hc128_ivsetup(&ctx, iv); results[0].iv_setup_us = timer.stop() * 1e6;
        for (int i = 0; i < 5; i++) {
            hc128_keysetup(&ctx, key, 256, 128);
            hc128_ivsetup(&ctx, iv);
            timer.start(); hc128_encrypt_bytes(&ctx, plain.data(), cipher.data(), sizes[i]); results[0].speeds[i] = timer.stop() * 1e6;
        }
        results[0].mbps = (sizes[4] * 8) / (results[0].speeds[4] / 1e6) / 1e6;
    }
    
    // Rabbit
    {
        RABBIT_ctx ctx;
        results[1].name = "Rabbit";
        timer.start(); rabbit_keysetup(&ctx, key, 256, 128); results[1].key_setup_us = timer.stop() * 1e6;
        timer.start(); rabbit_ivsetup(&ctx, iv); results[1].iv_setup_us = timer.stop() * 1e6;
        for (int i = 0; i < 5; i++) {
            rabbit_keysetup(&ctx, key, 256, 128);
            rabbit_ivsetup(&ctx, iv);
            timer.start(); rabbit_encrypt_bytes(&ctx, plain.data(), cipher.data(), sizes[i]); results[1].speeds[i] = timer.stop() * 1e6;
        }
        results[1].mbps = (sizes[4] * 8) / (results[1].speeds[4] / 1e6) / 1e6;
    }
    
    // Salsa20
    {
        SALSA20_ctx ctx;
        results[2].name = "Salsa20";
        timer.start(); salsa20_keysetup(&ctx, key, 256, 128); results[2].key_setup_us = timer.stop() * 1e6;
        timer.start(); salsa20_ivsetup(&ctx, iv); results[2].iv_setup_us = timer.stop() * 1e6;
        for (int i = 0; i < 5; i++) {
            salsa20_keysetup(&ctx, key, 256, 128);
            salsa20_ivsetup(&ctx, iv);
            timer.start(); salsa20_encrypt_bytes(&ctx, plain.data(), cipher.data(), sizes[i]); results[2].speeds[i] = timer.stop() * 1e6;
        }
        results[2].mbps = (sizes[4] * 8) / (results[2].speeds[4] / 1e6) / 1e6;
    }
    
    // Sosemanuk
    {
        SOSEMANUK_ctx ctx;
        results[3].name = "Sosemanuk";
        timer.start(); sosemanuk_keysetup(&ctx, key, 256, 128); results[3].key_setup_us = timer.stop() * 1e6;
        timer.start(); sosemanuk_ivsetup(&ctx, iv); results[3].iv_setup_us = timer.stop() * 1e6;
        for (int i = 0; i < 5; i++) {
            sosemanuk_keysetup(&ctx, key, 256, 128);
            sosemanuk_ivsetup(&ctx, iv);
            timer.start(); sosemanuk_encrypt_bytes(&ctx, plain.data(), cipher.data(), sizes[i]); results[3].speeds[i] = timer.stop() * 1e6;
        }
        results[3].mbps = (sizes[4] * 8) / (results[3].speeds[4] / 1e6) / 1e6;
    }
    
    cout << left << setw(12) << "Cipher";
    cout << right;
    cout << setw(14) << "Key(us)";
    cout << setw(14) << "IV(us)";
    cout << setw(12) << "1KB(us)";
    cout << setw(12) << "10KB(us)";
    cout << setw(12) << "100KB(us)";
    cout << setw(12) << "1MB(us)";
    cout << setw(12) << "10MB(us)";
    cout << setw(14) << "Speed(Mbps)" << endl;
    cout << string(114, '-') << endl;
    
    for (const auto& r : results) {
        cout << left << setw(12) << r.name;
        cout << right;
        cout << setw(14) << fixed << setprecision(2) << r.key_setup_us;
        cout << setw(14) << fixed << setprecision(2) << r.iv_setup_us;
        cout << setw(12) << fixed << setprecision(2) << r.speeds[0];
        cout << setw(12) << fixed << setprecision(2) << r.speeds[1];
        cout << setw(12) << fixed << setprecision(2) << r.speeds[2];
        cout << setw(12) << fixed << setprecision(2) << r.speeds[3];
        cout << setw(12) << fixed << setprecision(2) << r.speeds[4];
        cout << setw(14) << fixed << setprecision(2) << r.mbps << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Analysis:" << endl;
    cout << "========================================" << endl;
    
    auto fastest = min_element(results.begin(), results.end(),
        [](const Result& a, const Result& b) { return a.speeds[4] < b.speeds[4]; });
    cout << "Fastest for large data: " << fastest->name << " (" << fastest->mbps << " Mbps)" << endl;
    
    auto fastest_init = min_element(results.begin(), results.end(),
        [](const Result& a, const Result& b) { 
            return (a.key_setup_us + a.iv_setup_us) < (b.key_setup_us + b.iv_setup_us);
        });
    cout << "Fastest initialization: " << fastest_init->name 
         << " (" << (fastest_init->key_setup_us + fastest_init->iv_setup_us) << " us)" << endl;
    
    return 0;
}
