#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>
#include "utils.h"

extern "C" {
#include "../ciphers/salsa20.h"
}

using namespace std;

const int BUFFER_SIZE = 4096;

class FileTransfer {
private:
    SALSA20_ctx ctx;
    u8 key[32];
    u8 iv[16];
    
public:
    FileTransfer(const u8* key_bytes, const u8* iv_bytes) {
        memcpy(key, key_bytes, 32);
        memcpy(iv, iv_bytes, 16);
        ECRYPT_init();
        salsa20_keysetup(&ctx, key, 256, 128);
        salsa20_ivsetup(&ctx, iv);
    }
    
    void encrypt_file(const string& input_file, const string& output_file) {
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);
        if (!in.is_open()) throw runtime_error("Cannot open input file: " + input_file);
        
        in.seekg(0, ios::end);
        size_t total = in.tellg();
        in.seekg(0, ios::beg);
        
        vector<u8> buffer(BUFFER_SIZE);
        vector<u8> encrypted(BUFFER_SIZE);
        size_t processed = 0;
        
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) {
                salsa20_encrypt_bytes(&ctx, buffer.data(), encrypted.data(), bytes);
                out.write(reinterpret_cast<char*>(encrypted.data()), bytes);
                processed += bytes;
                print_progress(100.0 * processed / total);
            }
        }
        cout << "\nFile encrypted: " << input_file << " -> " << output_file << endl;
    }
    
    void decrypt_file(const string& input_file, const string& output_file) {
        salsa20_keysetup(&ctx, key, 256, 128);
        salsa20_ivsetup(&ctx, iv);
        
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);
        if (!in.is_open()) throw runtime_error("Cannot open input file: " + input_file);
        
        in.seekg(0, ios::end);
        size_t total = in.tellg();
        in.seekg(0, ios::beg);
        
        vector<u8> buffer(BUFFER_SIZE);
        vector<u8> decrypted(BUFFER_SIZE);
        size_t processed = 0;
        
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) {
                salsa20_encrypt_bytes(&ctx, buffer.data(), decrypted.data(), bytes);
                out.write(reinterpret_cast<char*>(decrypted.data()), bytes);
                processed += bytes;
                print_progress(100.0 * processed / total);
            }
        }
        cout << "\nFile decrypted: " << input_file << " -> " << output_file << endl;
    }
};

class NetworkTransfer {
private:
    int sock;
    struct sockaddr_in addr;
    
public:
    NetworkTransfer() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) throw runtime_error("Cannot create socket");
    }
    ~NetworkTransfer() { close(sock); }
    
    void start_server(int port) {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) throw runtime_error("Bind failed");
        if (listen(sock, 5) < 0) throw runtime_error("Listen failed");
        cout << "Server listening on port " << port << endl;
        
        int client_sock = accept(sock, nullptr, nullptr);
        if (client_sock < 0) throw runtime_error("Accept failed");
        
        u8 key[32], iv[16];
        recv(client_sock, key, 32, 0);
        recv(client_sock, iv, 16, 0);
        
        FileTransfer ft(key, iv);
        string filename = "received_encrypted.bin";
        ofstream out(filename, ios::binary);
        vector<u8> buffer(BUFFER_SIZE);
        ssize_t received;
        cout << "Receiving file..." << endl;
        while ((received = recv(client_sock, buffer.data(), BUFFER_SIZE, 0)) > 0) {
            out.write(reinterpret_cast<char*>(buffer.data()), received);
        }
        out.close();
        cout << "File received: " << filename << endl;
        ft.decrypt_file(filename, "decrypted_output.bin");
        close(client_sock);
    }
    
    void connect_to_server(const string& server_ip, int port, const string& filepath, const u8* key, const u8* iv) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, server_ip.c_str(), &addr.sin_addr);
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) throw runtime_error("Connection failed");
        
        send(sock, key, 32, 0);
        send(sock, iv, 16, 0);
        
        FileTransfer ft(key, iv);
        ft.encrypt_file(filepath, "temp_encrypted.bin");
        
        ifstream in("temp_encrypted.bin", ios::binary);
        vector<u8> buffer(BUFFER_SIZE);
        cout << "Sending file..." << endl;
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) send(sock, buffer.data(), bytes, 0);
        }
        in.close();
        cout << "File sent: " << filepath << endl;
    }
};

void print_usage(const char* prog_name) {
    cout << "Usage:\n";
    cout << "  " << prog_name << " --server [--port PORT]\n";
    cout << "  " << prog_name << " --client --ip IP --port PORT --file FILE --key KEY --iv IV\n";
    cout << "\nOptions:\n";
    cout << "  --server          Run as server\n";
    cout << "  --client          Run as client\n";
    cout << "  --ip IP           Server IP address\n";
    cout << "  --port PORT       Port number (default: 8080)\n";
    cout << "  --file FILE       File to transfer\n";
    cout << "  --key KEY         Encryption key (64 hex chars for 256 bits)\n";
    cout << "  --iv IV           Initialization vector (32 hex chars for 128 bits)\n";
    cout << "  --help            Show this help\n";
}

int main(int argc, char* argv[]) {
    bool is_server = false, is_client = false;
    string server_ip = "127.0.0.1";
    int port = 8080;
    string filename, key_hex, iv_hex;
    
    static struct option long_options[] = {
        {"server", no_argument, 0, 's'},
        {"client", no_argument, 0, 'c'},
        {"ip", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"file", required_argument, 0, 'f'},
        {"key", required_argument, 0, 'k'},
        {"iv", required_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "sci:p:f:k:v:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 's': is_server = true; break;
            case 'c': is_client = true; break;
            case 'i': server_ip = optarg; break;
            case 'p': port = atoi(optarg); break;
            case 'f': filename = optarg; break;
            case 'k': key_hex = optarg; break;
            case 'v': iv_hex = optarg; break;
            default: print_usage(argv[0]); return 0;
        }
    }
    
    try {
        if (is_server) {
            NetworkTransfer server;
            server.start_server(port);
        } else if (is_client) {
            if (filename.empty() || key_hex.empty() || iv_hex.empty()) {
                cerr << "Error: --file, --key, and --iv are required for client mode" << endl;
                print_usage(argv[0]);
                return 1;
            }
            if (key_hex.length() != 64) { cerr << "Error: Key must be 64 hex characters" << endl; return 1; }
            if (iv_hex.length() != 32) { cerr << "Error: IV must be 32 hex characters" << endl; return 1; }
            
            vector<u8> key_bytes = hex_to_bytes(key_hex);
            vector<u8> iv_bytes = hex_to_bytes(iv_hex);
            
            NetworkTransfer client;
            client.connect_to_server(server_ip, port, filename, key_bytes.data(), iv_bytes.data());
        } else {
            print_usage(argv[0]);
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
