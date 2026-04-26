#ifndef SALSA20_H
#define SALSA20_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t input[16];
    uint8_t keystream[64];
    size_t pos;
} Salsa20_ctx;

void Salsa20_init(void);
void Salsa20_keysetup(Salsa20_ctx* ctx, const uint8_t* key, size_t keybits);
void Salsa20_ivsetup(Salsa20_ctx* ctx, const uint8_t* iv);
void Salsa20_encrypt_bytes(Salsa20_ctx* ctx, const uint8_t* plaintext, uint8_t* ciphertext, size_t msglen);

#ifdef __cplusplus
}
#endif

#endif