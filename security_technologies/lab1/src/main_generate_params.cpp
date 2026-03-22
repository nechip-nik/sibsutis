#include "crypto_utils.h"
#include <iostream>

int main() {
    std::cout << "=== ГЕНЕРАЦИЯ ПАРАМЕТРОВ ДЛЯ ПРОТОКОЛОВ ===\n\n";
    
    // Параметры для мультипликативной группы
    DHParams mult_params;
    std::cout << "1. Параметры для мультипликативной группы Zp*\n";
    generate_multiplicative_group_params(mult_params, 256);
    mult_params.saveToFile("params_mult.txt");
    std::cout << "   Сохранено в params_mult.txt\n\n";
    
    // Параметры для циклической подгруппы
    DHParams sub_params;
    std::cout << "2. Параметры для циклической подгруппы порядка q\n";
    generate_subgroup_params(sub_params, 256);
    sub_params.saveToFile("params_sub.txt");
    std::cout << "   Сохранено в params_sub.txt\n\n";
    
    std::cout << "Генерация завершена!\n";
    return 0;
}