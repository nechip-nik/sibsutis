#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>
#include "ecdh.h"
#include "sha3.h"
#include "salsa20.h"
#include "rdtsc.h"
#include "utils.h"

#define PORT 8080
#define BUFFER_SIZE 4096

using namespace std;

int main() {
    cout << "=== SERVER (F) ===\n";
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }
    
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }
    
    cout << "Waiting for client connection...\n";
    
    int addrlen = sizeof(address);
    int client_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
    if (client_socket < 0) {
        perror("accept");
        return 1;
    }
    
    cout << "Client connected!\n\n";
    
    // ========== ECDH Key Exchange ==========
    cout << "--- ECDH Key Exchange ---\n";
    
    ECDH_P256 ecdh;
    mpz_t f_private, f_pubX, f_pubY;
    mpz_inits(f_private, f_pubX, f_pubY, nullptr);
    
    uint64_t start, end;
    
    start = rdtsc();
    ecdh.generateKeyPair(f_private, f_pubX, f_pubY);
    end = rdtsc();
    cout << "Key generation time: " << (end - start) << " cycles\n";
    
    uint8_t Fx[32], Fy[32];
    ecdh.exportPublicKey(f_pubX, f_pubY, Fx, Fy);
    send(client_socket, Fx, 32, 0);
    send(client_socket, Fy, 32, 0);
    
    uint8_t Cx[32], Cy[32];
    recv(client_socket, Cx, 32, MSG_WAITALL);
    recv(client_socket, Cy, 32, MSG_WAITALL);
    
    mpz_t C_pubX, C_pubY;
    mpz_inits(C_pubX, C_pubY, nullptr);
    ecdh.importPublicKey(Cx, Cy, C_pubX, C_pubY);
    
    mpz_t sharedSecret;
    mpz_init(sharedSecret);
    
    start = rdtsc();
    ecdh.computeSharedSecret(f_private, C_pubX, C_pubY, sharedSecret);
    end = rdtsc();
    cout << "Shared secret computation time: " << (end - start) << " cycles\n";
    
    auto secretBytes = mpzToVector(sharedSecret);
    auto keyBytes = SHA3::hash(secretBytes);
    cout << "Shared key: " << bytesToHex(keyBytes.data(), 32) << "\n\n";
    
    // ========== File Reception and Decryption ==========
    cout << "--- Receiving Encrypted File ---\n";
    
    uint8_t iv[8];
    uint32_t fileSizeNet;
    recv(client_socket, iv, 8, MSG_WAITALL);
    recv(client_socket, &fileSizeNet, 4, MSG_WAITALL);
    
    uint32_t fileSize = ntohl(fileSizeNet);
    cout << "File size: " << fileSize << " bytes\n";
    
    Salsa20_ctx ctx;
    Salsa20_init();
    Salsa20_keysetup(&ctx, keyBytes.data(), 256);
    Salsa20_ivsetup(&ctx, iv);
    
    ofstream outFile("received_file.txt", ios::binary);
    vector<uint8_t> encrypted(BUFFER_SIZE);
    vector<uint8_t> decrypted(BUFFER_SIZE);
    
    start = rdtsc();
    
    uint32_t remaining = fileSize;
    while (remaining > 0) {
        size_t chunk = min((uint32_t)BUFFER_SIZE, remaining);
        recv(client_socket, encrypted.data(), chunk, MSG_WAITALL);
        Salsa20_encrypt_bytes(&ctx, encrypted.data(), decrypted.data(), chunk);
        outFile.write((char*)decrypted.data(), chunk);
        remaining -= chunk;
    }
    
    end = rdtsc();
    cout << "Decryption time: " << (end - start) << " cycles\n";
    cout << "File saved as 'received_file.txt'\n";
    
    outFile.close();
    close(client_socket);
    close(server_fd);
    
    mpz_clears(f_private, f_pubX, f_pubY, C_pubX, C_pubY, sharedSecret, nullptr);
    
    cout << "\nServer finished.\n";
    return 0;
}