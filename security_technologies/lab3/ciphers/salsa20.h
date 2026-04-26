#ifndef SALSA20_H
#define SALSA20_H

#include "ecrypt-sync.h"

typedef struct {
    u32 state[16];
    u8 buffer[64];
    int pos;
} SALSA20_ctx;

void salsa20_keysetup(SALSA20_ctx* ctx, const u8* key, u32 keysize, u32 ivsize);
void salsa20_ivsetup(SALSA20_ctx* ctx, const u8* iv);
void salsa20_encrypt_bytes(SALSA20_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen);

#endif
