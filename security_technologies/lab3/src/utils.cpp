#include "utils.h"
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;

string bytes_to_hex(const uint8_t* bytes, size_t len) {
    stringstream ss;
    ss << hex << setfill('0');
    for (size_t i = 0; i < len; i++) {
        ss << setw(2) << (int)bytes[i];
    }
    return ss.str();
}

vector<uint8_t> hex_to_bytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byte_str = hex.substr(i, 2);
        uint8_t byte = stoi(byte_str, nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

void print_progress(double percentage) {
    int bar_width = 50;
    int pos = (int)(bar_width * percentage / 100.0);
    
    cout << "[";
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << fixed << setprecision(1) << percentage << "%\r";
    cout.flush();
}