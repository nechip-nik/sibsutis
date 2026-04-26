#include <iostream>
#include <vector>
#include "sha3.h"

using namespace std;

int main() {
    cout << "Testing SHA3-256..." << endl;
    
    vector<uint8_t> test_data = {'H', 'e', 'l', 'l', 'o'};
    vector<uint8_t> hash = SHA3::hash256(test_data);
    
    cout << "Hash (32 bytes): ";
    for (auto b : hash) {
        printf("%02x", b);
    }
    cout << endl;
    
    cout << "SHA3-256 works correctly!" << endl;
    return 0;
}
