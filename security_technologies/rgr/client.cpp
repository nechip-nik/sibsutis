#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <random>
#include "ecdh.h"
#include "sha3.h"
#include "salsa20.h"
#include "rdtsc.h"
#include "utils.h"

#define PORT 8080
#define BUFFER_SIZE 4096

using namespace std;

int main() {
    cout << "=== CLIENT (C) ===\n";
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation error");
        return 1;
    }
    
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return 1;
    }
    
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return 1;
    }
    
    cout << "Connected to server!\n\n";
    
    // ========== ECDH Key Exchange ==========
    cout << "--- ECDH Key Exchange ---\n";
    
    ECDH_P256 ecdh;
    mpz_t c_private, c_pubX, c_pubY;
    mpz_inits(c_private, c_pubX, c_pubY, nullptr);
    
    uint64_t start, end;
    
    start = rdtsc();
    ecdh.generateKeyPair(c_private, c_pubX, c_pubY);
    end = rdtsc();
    cout << "Key generation time: " << (end - start) << " cycles\n";
    
    uint8_t Fx[32], Fy[32];
    recv(sock, Fx, 32, MSG_WAITALL);
    recv(sock, Fy, 32, MSG_WAITALL);
    
    mpz_t F_pubX, F_pubY;
    mpz_inits(F_pubX, F_pubY, nullptr);
    ecdh.importPublicKey(Fx, Fy, F_pubX, F_pubY);
    
    uint8_t Cx[32], Cy[32];
    ecdh.exportPublicKey(c_pubX, c_pubY, Cx, Cy);
    send(sock, Cx, 32, 0);
    send(sock, Cy, 32, 0);
    
    mpz_t sharedSecret;
    mpz_init(sharedSecret);
    
    start = rdtsc();
    ecdh.computeSharedSecret(c_private, F_pubX, F_pubY, sharedSecret);
    end = rdtsc();
    cout << "Shared secret computation time: " << (end - start) << " cycles\n";
    
    auto secretBytes = mpzToVector(sharedSecret);
    auto keyBytes = SHA3::hash(secretBytes);
    cout << "Shared key: " << bytesToHex(keyBytes.data(), 32) << "\n\n";
    
    // ========== File Encryption and Transmission ==========
    cout << "--- Sending Encrypted File ---\n";
    
    ifstream inFile("test.txt", ios::binary | ios::ate);
    if (!inFile.is_open()) {
        ofstream testFile("test.txt");
        testFile << "This is a secret message!\nLine 2\nLine 3\n";
        testFile.close();
        inFile.open("test.txt", ios::binary | ios::ate);
    }
    
    streamsize fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);
    
    vector<uint8_t> plaintext(fileSize);
    inFile.read((char*)plaintext.data(), fileSize);
    inFile.close();
    
    cout << "File size: " << fileSize << " bytes\n";
    
    uint8_t iv[8];
    random_device rd;
    for (int i = 0; i < 8; i++) iv[i] = rd() & 0xFF;
    
    Salsa20_ctx ctx;
    Salsa20_init();
    Salsa20_keysetup(&ctx, keyBytes.data(), 256);
    Salsa20_ivsetup(&ctx, iv);
    
    vector<uint8_t> ciphertext(fileSize);
    
    start = rdtsc();
    Salsa20_encrypt_bytes(&ctx, plaintext.data(), ciphertext.data(), fileSize);
    end = rdtsc();
    cout << "Encryption time: " << (end - start) << " cycles\n";
    
    send(sock, iv, 8, 0);
    uint32_t sizeNet = htonl(static_cast<uint32_t>(fileSize));
    send(sock, &sizeNet, 4, 0);
    send(sock, ciphertext.data(), fileSize, 0);
    
    cout << "Encrypted file sent!\n";
    
    close(sock);
    mpz_clears(c_private, c_pubX, c_pubY, F_pubX, F_pubY, sharedSecret, nullptr);
    
    cout << "\nClient finished.\n";
    return 0;
}