#include "salsa20.h"
#include <string.h>

#define ROTL32(a, b) (((a) << (b)) | ((a) >> (32 - (b))))

#define QUARTERROUND(a, b, c, d) \
    b ^= ROTL32((a + d), 7);  \
    c ^= ROTL32((b + a), 9);  \
    d ^= ROTL32((c + b), 13); \
    a ^= ROTL32((d + c), 18);

static void salsa20_block(SALSA20_ctx* ctx, u8* output) {
    u32 x[16];
    int i;
    memcpy(x, ctx->state, sizeof(x));
    for (i = 0; i < 10; i++) {
        QUARTERROUND(x[0], x[4], x[8],  x[12]);
        QUARTERROUND(x[5], x[9], x[13], x[1]);
        QUARTERROUND(x[10], x[14], x[2], x[6]);
        QUARTERROUND(x[15], x[3], x[7], x[11]);
        QUARTERROUND(x[0], x[1], x[2], x[3]);
        QUARTERROUND(x[5], x[6], x[7], x[4]);
        QUARTERROUND(x[10], x[11], x[8], x[9]);
        QUARTERROUND(x[15], x[12], x[13], x[14]);
    }
    for (i = 0; i < 16; i++) { x[i] += ctx->state[i]; }
    for (i = 0; i < 16; i++) {
        output[4*i]   = x[i] & 0xFF;
        output[4*i+1] = (x[i] >> 8) & 0xFF;
        output[4*i+2] = (x[i] >> 16) & 0xFF;
        output[4*i+3] = (x[i] >> 24) & 0xFF;
    }
    ctx->state[8] = (ctx->state[8] + 1) & 0xFFFFFFFF;
    if (ctx->state[8] == 0) ctx->state[9] = (ctx->state[9] + 1) & 0xFFFFFFFF;
}

void salsa20_keysetup(SALSA20_ctx* ctx, const u8* key, u32 keysize, u32 ivsize) {
    int i;
    (void)ivsize;
    memset(ctx->state, 0, sizeof(ctx->state));
    ctx->state[0] = 0x61707865; ctx->state[5] = 0x3320646e;
    ctx->state[10] = 0x79622d32; ctx->state[15] = 0x6b206574;
    if (keysize == 256) {
        for (i = 0; i < 8; i++) {
            ctx->state[1+i] = ((u32)key[4*i+3] << 24) | ((u32)key[4*i+2] << 16) |
                              ((u32)key[4*i+1] << 8) | (u32)key[4*i];
            ctx->state[11+i] = ctx->state[1+i];
        }
    } else if (keysize == 128) {
        for (i = 0; i < 4; i++) {
            ctx->state[1+i] = ((u32)key[4*i+3] << 24) | ((u32)key[4*i+2] << 16) |
                              ((u32)key[4*i+1] << 8) | (u32)key[4*i];
            ctx->state[11+i] = ctx->state[1+i];
            ctx->state[5+i] = ctx->state[1+i];
            ctx->state[15+i] = ctx->state[1+i];
        }
    }
    ctx->pos = 64;
}

void salsa20_ivsetup(SALSA20_ctx* ctx, const u8* iv) {
    u32 iv_val[2];
    iv_val[0] = ((u32)iv[3] << 24) | ((u32)iv[2] << 16) | ((u32)iv[1] << 8) | (u32)iv[0];
    iv_val[1] = ((u32)iv[7] << 24) | ((u32)iv[6] << 16) | ((u32)iv[5] << 8) | (u32)iv[4];
    ctx->state[6] = iv_val[0]; ctx->state[7] = iv_val[1];
    ctx->state[8] = 0; ctx->state[9] = 0;
    ctx->state[14] = iv_val[0]; ctx->state[13] = iv_val[1];
    ctx->state[12] = 0; ctx->state[11] = 0;
    ctx->pos = 64;
}

void salsa20_encrypt_bytes(SALSA20_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen) {
    u32 i;
    for (i = 0; i < msglen; i++) {
        if (ctx->pos >= 64) { salsa20_block(ctx, ctx->buffer); ctx->pos = 0; }
        if (plaintext) ciphertext[i] = plaintext[i] ^ ctx->buffer[ctx->pos];
        else ciphertext[i] = ctx->buffer[ctx->pos];
        ctx->pos++;
    }
}
