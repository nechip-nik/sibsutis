#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <getopt.h>
#include <random>
#include "sha3.h"
#include "utils.h"

using namespace std;

const int BUFFER_SIZE = 4096;

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

// Простой потоковый шифр (XOR с хешем)
class StreamCipher {
private:
    vector<uint8_t> key;
    vector<uint8_t> iv;
    size_t counter;
    
public:
    StreamCipher(const vector<uint8_t>& key_bytes, const vector<uint8_t>& iv_bytes) 
        : key(key_bytes), iv(iv_bytes), counter(0) {}
    
    void generate_keystream(uint8_t* output, size_t len) {
        vector<uint8_t> data;
        data.insert(data.end(), key.begin(), key.end());
        data.insert(data.end(), iv.begin(), iv.end());
        
        for (size_t i = 0; i < len; i++) {
            vector<uint8_t> counter_bytes;
            size_t c = counter + i;
            for (int j = 0; j < 8; j++) {
                counter_bytes.push_back((c >> (j * 8)) & 0xFF);
            }
            
            vector<uint8_t> hash_input = data;
            hash_input.insert(hash_input.end(), counter_bytes.begin(), counter_bytes.end());
            
            vector<uint8_t> hash = SHA3::hash256(hash_input);
            output[i] = hash[0];
        }
        counter += len;
    }
    
    void encrypt(const uint8_t* plaintext, uint8_t* ciphertext, size_t len) {
        vector<uint8_t> keystream(len);
        generate_keystream(keystream.data(), len);
        
        for (size_t i = 0; i < len; i++) {
            ciphertext[i] = plaintext[i] ^ keystream[i];
        }
    }
    
    void decrypt(const uint8_t* ciphertext, uint8_t* plaintext, size_t len) {
        encrypt(ciphertext, plaintext, len); // XOR симметричен
    }
};

// Файловый менеджер с шифрованием и хешированием
class FileManager {
private:
    StreamCipher cipher;
    vector<uint8_t> file_hash;
    
public:
    FileManager(const vector<uint8_t>& key, const vector<uint8_t>& iv) 
        : cipher(key, iv) {}
    
    void encrypt_file(const string& input_file, const string& output_file) {
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);
        
        if (!in.is_open()) {
            throw runtime_error("Cannot open input file: " + input_file);
        }
        
        // Вычисляем хеш исходного файла
        in.seekg(0, ios::end);
        size_t file_size = in.tellg();
        in.seekg(0, ios::beg);
        
        vector<uint8_t> file_data(file_size);
        in.read(reinterpret_cast<char*>(file_data.data()), file_size);
        
        Timer hash_timer;
        hash_timer.start();
        file_hash = SHA3::hash256(file_data);
        double hash_time = hash_timer.stop();
        
        cout << "File hash (SHA3-256): " << bytes_to_hex(file_hash.data(), file_hash.size()) << endl;
        cout << "Hash computation time: " << hash_time * 1e6 << " μs" << endl;
        
        // Шифруем файл
        in.clear();
        in.seekg(0, ios::beg);
        
        vector<uint8_t> buffer(BUFFER_SIZE);
        vector<uint8_t> encrypted(BUFFER_SIZE);
        size_t processed = 0;
        
        // Записываем хеш в начало зашифрованного файла
        out.write(reinterpret_cast<const char*>(file_hash.data()), file_hash.size());
        
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) {
                cipher.encrypt(buffer.data(), encrypted.data(), bytes);
                out.write(reinterpret_cast<char*>(encrypted.data()), bytes);
                processed += bytes;
                print_progress(100.0 * processed / file_size);
            }
        }
        
        cout << "\nFile encrypted: " << input_file << " -> " << output_file << endl;
    }
    
    bool decrypt_file(const string& input_file, const string& output_file) {
        ifstream in(input_file, ios::binary);
        ofstream out(output_file, ios::binary);
        
        if (!in.is_open()) {
            throw runtime_error("Cannot open input file: " + input_file);
        }
        
        // Считываем хеш из начала файла
        vector<uint8_t> saved_hash(32);
        in.read(reinterpret_cast<char*>(saved_hash.data()), 32);
        
        // Получаем размер зашифрованных данных
        in.seekg(0, ios::end);
        streampos end_pos = in.tellg();
        size_t encrypted_size = static_cast<size_t>(end_pos) - 32;
        in.seekg(32, ios::beg);
        
        // Расшифровываем
        vector<uint8_t> buffer(BUFFER_SIZE);
        vector<uint8_t> decrypted(BUFFER_SIZE);
        vector<uint8_t> all_data;
        all_data.reserve(encrypted_size);
        
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) {
                cipher.decrypt(buffer.data(), decrypted.data(), bytes);
                out.write(reinterpret_cast<char*>(decrypted.data()), bytes);
                all_data.insert(all_data.end(), decrypted.data(), decrypted.data() + bytes);
            }
        }
        
        // Проверяем хеш расшифрованного файла
        Timer hash_timer;
        hash_timer.start();
        vector<uint8_t> computed_hash = SHA3::hash256(all_data);
        double hash_time = hash_timer.stop();
        
        cout << "Hash computation time: " << hash_time * 1e6 << " μs" << endl;
        
        if (computed_hash == saved_hash) {
            cout << "✓ Hash verification PASSED! File integrity confirmed." << endl;
            cout << "File decrypted: " << input_file << " -> " << output_file << endl;
            return true;
        } else {
            cout << "✗ Hash verification FAILED! File may be corrupted." << endl;
            return false;
        }
    }
};

// Сетевой менеджер
class NetworkManager {
private:
    int sock;
    struct sockaddr_in addr;
    
public:
    NetworkManager() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) throw runtime_error("Cannot create socket");
    }
    
    ~NetworkManager() { close(sock); }
    
    void start_server(int port) {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        
        if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            throw runtime_error("Bind failed");
        }
        
        if (listen(sock, 5) < 0) {
            throw runtime_error("Listen failed");
        }
        
        cout << "Server listening on port " << port << endl;
        
        int client_sock = accept(sock, nullptr, nullptr);
        if (client_sock < 0) throw runtime_error("Accept failed");
        
        // Получаем ключ и IV
        vector<uint8_t> key(32), iv(16);
        recv(client_sock, key.data(), 32, 0);
        recv(client_sock, iv.data(), 16, 0);
        
        FileManager fm(key, iv);
        
        string filename = "received_encrypted.bin";
        ofstream out(filename, ios::binary);
        vector<uint8_t> buffer(BUFFER_SIZE);
        ssize_t received;
        
        cout << "Receiving file..." << endl;
        while ((received = recv(client_sock, buffer.data(), BUFFER_SIZE, 0)) > 0) {
            out.write(reinterpret_cast<char*>(buffer.data()), received);
        }
        out.close();
        cout << "File received: " << filename << endl;
        
        fm.decrypt_file(filename, "decrypted_output.bin");
        
        close(client_sock);
    }
    
    void connect_to_server(const string& server_ip, int port, const string& filepath,
                          const vector<uint8_t>& key, const vector<uint8_t>& iv) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, server_ip.c_str(), &addr.sin_addr);
        
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            throw runtime_error("Connection failed");
        }
        
        send(sock, key.data(), 32, 0);
        send(sock, iv.data(), 16, 0);
        
        FileManager fm(key, iv);
        fm.encrypt_file(filepath, "temp_encrypted.bin");
        
        ifstream in("temp_encrypted.bin", ios::binary);
        vector<uint8_t> buffer(BUFFER_SIZE);
        
        cout << "Sending file..." << endl;
        while (in.good()) {
            in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytes = in.gcount();
            if (bytes > 0) {
                send(sock, buffer.data(), bytes, 0);
            }
        }
        in.close();
        cout << "File sent: " << filepath << endl;
    }
};

// Генерация случайного ключа и IV
void generate_key_iv(vector<uint8_t>& key, vector<uint8_t>& iv) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    key.resize(32);
    iv.resize(16);
    
    for (int i = 0; i < 32; i++) key[i] = dis(gen);
    for (int i = 0; i < 16; i++) iv[i] = dis(gen);
}

void print_usage(const char* prog_name) {
    cout << "Usage:" << endl;
    cout << "  " << prog_name << " --server [--port PORT]" << endl;
    cout << "  " << prog_name << " --client --ip IP --port PORT --file FILE" << endl;
    cout << "  " << prog_name << " --benchmark" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << "  --server          Run as server" << endl;
    cout << "  --client          Run as client" << endl;
    cout << "  --benchmark       Run hash function benchmark" << endl;
    cout << "  --ip IP           Server IP address" << endl;
    cout << "  --port PORT       Port number (default: 8080)" << endl;
    cout << "  --file FILE       File to transfer" << endl;
    cout << "  --help            Show this help" << endl;
}

void run_benchmark() {
    cout << "========================================" << endl;
    cout << "SHA-3 Hash Function Benchmark" << endl;
    cout << "========================================" << endl;
    
    vector<size_t> sizes = {1024, 10240, 102400, 1048576, 10485760};
    vector<uint8_t> data(sizes.back());
    
    // Заполняем случайными данными
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < sizes.back(); i++) {
        data[i] = dis(gen);
    }
    
    Timer timer;
    
    cout << left << setw(15) << "Data Size";
    cout << right;
    cout << setw(20) << "SHA3-256 Time(us)";
    cout << setw(20) << "SHA3-512 Time(us)";
    cout << setw(20) << "Speed(MB/s)" << endl;
    cout << string(75, '-') << endl;
    
    for (size_t size : sizes) {
        // SHA3-256
        timer.start();
        vector<uint8_t> hash256 = SHA3::hash256(data.data(), size);
        double time256 = timer.stop() * 1e6;
        
        // SHA3-512
        timer.start();
        vector<uint8_t> hash512 = SHA3::hash512(data.data(), size);
        double time512 = timer.stop() * 1e6;
        
        double speed = size / (time256 / 1e6) / (1024 * 1024);
        
        cout << left << setw(15) << size;
        cout << right;
        cout << setw(20) << fixed << setprecision(2) << time256;
        cout << setw(20) << fixed << setprecision(2) << time512;
        cout << setw(20) << fixed << setprecision(2) << speed << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Hash vs Network Operations Comparison" << endl;
    cout << "========================================" << endl;
    
    // Сравнение с операциями шифрования
    vector<uint8_t> key(32), iv(16);
    generate_key_iv(key, iv);
    StreamCipher cipher(key, iv);
    
    vector<uint8_t> plain(1048576); // 1MB
    vector<uint8_t> encrypted(plain.size());
    
    timer.start();
    cipher.encrypt(plain.data(), encrypted.data(), plain.size());
    double encrypt_time = timer.stop() * 1e6;
    
    timer.start();
    vector<uint8_t> hash = SHA3::hash256(plain);
    double hash_time = timer.stop() * 1e6;
    
    cout << "Operation (1 MB data):" << endl;
    cout << "  Encryption time: " << encrypt_time << " us" << endl;
    cout << "  Hash time: " << hash_time << " us" << endl;
    cout << "  Ratio (hash/encrypt): " << (hash_time / encrypt_time) << endl;
}

int main(int argc, char* argv[]) {
    bool is_server = false;
    bool is_client = false;
    bool benchmark = false;
    string server_ip = "127.0.0.1";
    int port = 8080;
    string filename;
    
    static struct option long_options[] = {
        {"server", no_argument, 0, 's'},
        {"client", no_argument, 0, 'c'},
        {"benchmark", no_argument, 0, 'b'},
        {"ip", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"file", required_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "scbi:p:f:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 's': is_server = true; break;
            case 'c': is_client = true; break;
            case 'b': benchmark = true; break;
            case 'i': server_ip = optarg; break;
            case 'p': port = atoi(optarg); break;
            case 'f': filename = optarg; break;
            default: print_usage(argv[0]); return 0;
        }
    }
    
    try {
        if (benchmark) {
            run_benchmark();
        } else if (is_server) {
            NetworkManager server;
            server.start_server(port);
        } else if (is_client) {
            if (filename.empty()) {
                cerr << "Error: --file is required for client mode" << endl;
                print_usage(argv[0]);
                return 1;
            }
            
            vector<uint8_t> key, iv;
            generate_key_iv(key, iv);
            
            cout << "Generated key: " << bytes_to_hex(key.data(), key.size()) << endl;
            cout << "Generated IV: " << bytes_to_hex(iv.data(), iv.size()) << endl;
            
            NetworkManager client;
            client.connect_to_server(server_ip, port, filename, key, iv);
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
