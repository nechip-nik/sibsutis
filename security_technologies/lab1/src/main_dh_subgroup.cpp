#include "crypto_utils.h"
#include <iostream>

int main() {
    std::cout << "=== ДИФФИ-ХЕЛЛМАН В ЦИКЛИЧЕСКОЙ ПОДГРУППЕ ===\n\n";
    
    DHParams params;
    params.loadFromFile("params_sub.txt");
    
    std::cout << "Используемые параметры:\n";
    params.print();
    std::cout << std::endl;
    
    TimingResult timing;
    diffie_hellman_subgroup(params, timing);
    print_timing("DH Subgroup", timing);
    
    return 0;
}