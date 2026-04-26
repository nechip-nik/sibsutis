#include "sosemanuk.h"
#include <string.h>

#define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

static void next_state(SOSEMANUK_ctx* ctx) {
    u32 t1, t2, t3, t4, t5, t6;
    t1 = ctx->state[0] ^ (ctx->state[1] >> 12) ^ (ctx->state[2] >> 4);
    t2 = ctx->state[4] ^ (ctx->state[5] >> 12) ^ (ctx->state[6] >> 4);
    t3 = ctx->state[8] ^ (ctx->state[9] >> 12) ^ (ctx->state[3] >> 4);
    t4 = ctx->state[2] ^ (ctx->state[3] >> 12) ^ (ctx->state[0] >> 4);
    t5 = ctx->state[6] ^ (ctx->state[7] >> 12) ^ (ctx->state[4] >> 4);
    t6 = ctx->state[9] ^ (ctx->state[0] >> 12) ^ (ctx->state[1] >> 4);
    ctx->state[0] = t1; ctx->state[4] = t2; ctx->state[8] = t3;
    ctx->state[2] = t4; ctx->state[6] = t5; ctx->state[9] = t6;
    ctx->keystream[0] = ctx->state[0] ^ ctx->state[4] ^ ctx->state[8];
    ctx->keystream[1] = ctx->state[1] ^ ctx->state[5] ^ ctx->state[6];
    ctx->keystream[2] = ctx->state[2] ^ ctx->state[6] ^ ctx->state[7];
    ctx->keystream[3] = ctx->state[3] ^ ctx->state[7] ^ ctx->state[8];
}

void sosemanuk_keysetup(SOSEMANUK_ctx* ctx, const u8* key, u32 keysize, u32 ivsize) {
    int i;
    (void)ivsize;
    for (i = 0; i < 10; i++) ctx->state[i] = 0;
    for (i = 0; i < keysize / 32; i++) {
        ctx->state[i] = ((u32)key[4*i+3] << 24) | ((u32)key[4*i+2] << 16) |
                        ((u32)key[4*i+1] << 8) | (u32)key[4*i];
    }
    ctx->r1 = ctx->r2 = 0;
    ctx->pos = 4;
}

void sosemanuk_ivsetup(SOSEMANUK_ctx* ctx, const u8* iv) {
    u32 iv0, iv1;
    iv0 = ((u32)iv[3] << 24) | ((u32)iv[2] << 16) | ((u32)iv[1] << 8) | (u32)iv[0];
    iv1 = ((u32)iv[7] << 24) | ((u32)iv[6] << 16) | ((u32)iv[5] << 8) | (u32)iv[4];
    ctx->state[4] ^= iv0; ctx->state[5] ^= iv1;
    ctx->state[6] ^= iv0; ctx->state[7] ^= iv1;
    ctx->state[8] ^= iv0; ctx->state[9] ^= iv1;
    ctx->pos = 4;
}

void sosemanuk_encrypt_bytes(SOSEMANUK_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen) {
    u32 i;
    u8* ks_bytes;
    for (i = 0; i < msglen; i++) {
        if (ctx->pos >= 4) { next_state(ctx); ctx->pos = 0; }
        ks_bytes = (u8*)&ctx->keystream[ctx->pos / 4];
        if (plaintext) ciphertext[i] = plaintext[i] ^ ks_bytes[ctx->pos % 4];
        else ciphertext[i] = ks_bytes[ctx->pos % 4];
        ctx->pos++;
    }
}
