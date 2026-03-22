#include "crypto_utils.h"
#include <iostream>

int main() {
    std::cout << "=== ОРИГИНАЛЬНЫЙ ПРОТОКОЛ ДИФФИ-ХЕЛЛМАНА ===\n\n";
    
    DHParams params;
    params.loadFromFile("params_mult.txt");
    
    std::cout << "Используемые параметры:\n";
    params.print();
    std::cout << std::endl;
    
    TimingResult timing;
    diffie_hellman_original(params, timing);
    print_timing("DH Original", timing);
    
    return 0;
}