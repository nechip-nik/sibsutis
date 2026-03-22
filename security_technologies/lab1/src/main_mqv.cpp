#include "crypto_utils.h"
#include <iostream>

int main() {
    std::cout << "=== ПРОТОКОЛ MQV (MENEZES-QU-VANSTONE) ===\n\n";
    
    DHParams params;
    params.loadFromFile("params_sub.txt");
    
    std::cout << "Используемые параметры:\n";
    params.print();
    std::cout << std::endl;
    
    TimingResult timing;
    mqv_protocol(params, timing);
    print_timing("MQV Protocol", timing);
    
    return 0;
}