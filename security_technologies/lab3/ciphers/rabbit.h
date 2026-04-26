#ifndef RABBIT_H
#define RABBIT_H

#include "ecrypt-sync.h"

typedef struct {
    u32 x[8];
    u32 c[8];
    u32 carry;
    u32 keystream[4];
    int pos;
} RABBIT_ctx;

void rabbit_keysetup(RABBIT_ctx* ctx, const u8* key, u32 keysize, u32 ivsize);
void rabbit_ivsetup(RABBIT_ctx* ctx, const u8* iv);
void rabbit_encrypt_bytes(RABBIT_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen);

#endif
