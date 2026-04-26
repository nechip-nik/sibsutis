#ifndef HC128_H
#define HC128_H

#include "ecrypt-sync.h"

typedef struct {
    u32 P[512];
    u32 Q[512];
    u32 counter;
    u32 key[8];
    u32 iv[8];
    u32 keystream[4];
    int pos;
} HC128_ctx;

void hc128_keysetup(HC128_ctx* ctx, const u8* key, u32 keysize, u32 ivsize);
void hc128_ivsetup(HC128_ctx* ctx, const u8* iv);
void hc128_encrypt_bytes(HC128_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen);

#endif
