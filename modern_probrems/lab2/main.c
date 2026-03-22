#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef enum {
    STATE_0,   
    STATE_1,     
    STATE_PLUS,  
    STATE_MINUS  
} QubitState;

typedef enum {
    BASIS_Z,     
    BASIS_X      
} Basis;

typedef enum {
    BIT_0,
    BIT_1
} SecretBit;

QubitState generate_qubit() {
    return rand() % 4;
}

Basis choose_basis() {
    return rand() % 2;
}

// Функция для измерения кубита в заданном базисе
SecretBit measure_qubit(QubitState qubit, Basis basis) {
    if (basis == BASIS_Z) {
        if (qubit == STATE_0) return BIT_0;
        if (qubit == STATE_1) return BIT_1;
        return rand() % 2 ? BIT_1 : BIT_0;
    } else {
        if (qubit == STATE_PLUS) return BIT_0;
        if (qubit == STATE_MINUS) return BIT_1;
        return rand() % 2 ? BIT_1 : BIT_0;
    }
}

// Функция для проверки совместимости базиса и состояния кубита
int is_basis_compatible(QubitState qubit, Basis basis) {
    if (basis == BASIS_Z) {
        return (qubit == STATE_0 || qubit == STATE_1);
    } else {
        return (qubit == STATE_PLUS || qubit == STATE_MINUS);
    }
}

// Функция для получения секретного бита из состояния кубита
SecretBit get_secret_bit(QubitState qubit) {
    if (qubit == STATE_0 || qubit == STATE_PLUS) {
        return BIT_0;
    } else {
        return BIT_1;
    }
}

void print_qubit_state(QubitState qubit) {
    switch(qubit) {
        case STATE_0: printf("|0⟩"); break;
        case STATE_1: printf("|1⟩"); break;
        case STATE_PLUS: printf("|+⟩"); break;
        case STATE_MINUS: printf("|-⟩"); break;
    }
}

void print_basis(Basis basis) {
    if (basis == BASIS_Z) {
        printf("Z-базис {|0⟩, |1⟩}");
    } else {
        printf("X-базис {|+⟩, |-⟩}");
    }
}

void print_secret_bit(SecretBit bit) {
    if (bit == BIT_0) {
        printf("0");
    } else {
        printf("1");
    }
}

int main() {
    srand(time(NULL));
    
    printf("=== МОДЕЛИРОВАНИЕ КВАНТОВОГО ПРОТОКОЛА ===\n\n");
    
    int success = 0;
    int attempts = 0;
    
    while (!success) {
        attempts++;
        printf("Попытка №%d:\n", attempts);

        QubitState alice_qubit = generate_qubit();
        printf("1. A генерирует кубит: ");
        print_qubit_state(alice_qubit);
        printf("\n");
        

        Basis bob_basis = choose_basis();
        printf("2. B выбирает базис: ");
        print_basis(bob_basis);
        printf("\n");
        

        SecretBit bob_measurement = measure_qubit(alice_qubit, bob_basis);
        printf("3. B измеряет кубит и получает: ");
        print_secret_bit(bob_measurement);
        printf("\n");
        

        printf("4. B сообщает A, что использовал ");
        print_basis(bob_basis);
        printf("\n");
        

        if (is_basis_compatible(alice_qubit, bob_basis)) {
            printf("5. A подтверждает: 'ОК' (базисы совпали)\n");
            
            // Определение общего секретного бита
            SecretBit secret_bit = get_secret_bit(alice_qubit);
            printf("6. ОБЩИЙ СЕКРЕТНЫЙ БИТ: ");
            print_secret_bit(secret_bit);
            printf("\n");
            
            success = 1;
        } else {
            printf("5. A сообщает: 'ПОВТОР' (базисы не совпали)\n");
            printf("--- Протокол повторяется ---\n\n");
        }
    }
    
    printf("\nПротокол успешно завершен за %d попыток!\n", attempts);
    
    return 0;
}