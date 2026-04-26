#ifndef SOSEMANUK_H
#define SOSEMANUK_H

#include "ecrypt-sync.h"

typedef struct {
    u32 state[10];
    u32 r1, r2;
    u32 keystream[4];
    int pos;
} SOSEMANUK_ctx;

void sosemanuk_keysetup(SOSEMANUK_ctx* ctx, const u8* key, u32 keysize, u32 ivsize);
void sosemanuk_ivsetup(SOSEMANUK_ctx* ctx, const u8* iv);
void sosemanuk_encrypt_bytes(SOSEMANUK_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen);

#endif
