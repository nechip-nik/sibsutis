#include "rabbit.h"
#include <string.h>

#define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

static u32 g(u32 x) {
    u32 a, b, h;
    a = x & 0xFFFF;
    b = x >> 16;
    h = ((((a * a) >> 17) + (a * b) >> 15) + ((((b * b) << 1) - a * b) >> 16) +
         ((((a * a) << 1) + a * b) >> 16) + ((((b * b) >> 15) + (a * b) << 1) >> 16)) & 0xFFFFFFFF;
    return h ^ (h << 16);
}

static void next_state(RABBIT_ctx* ctx) {
    u32 i, gx[8];
    for (i = 0; i < 8; i++) {
        gx[i] = g(ctx->x[i]);
        ctx->c[i] = (ctx->c[i] + 0x4D34D34D + (ctx->carry >> i)) & 0xFFFFFFFF;
    }
    ctx->carry = (ctx->carry + 1) & 0xFFFFFFFF;
    ctx->x[0] = gx[0] + ROTL32(gx[7], 16) + ROTL32(gx[6], 16);
    ctx->x[1] = gx[1] + ROTL32(gx[0], 8) + gx[7];
    ctx->x[2] = gx[2] + ROTL32(gx[1], 16) + ROTL32(gx[0], 16);
    ctx->x[3] = gx[3] + ROTL32(gx[2], 8) + gx[1];
    ctx->x[4] = gx[4] + ROTL32(gx[3], 16) + ROTL32(gx[2], 16);
    ctx->x[5] = gx[5] + ROTL32(gx[4], 8) + gx[3];
    ctx->x[6] = gx[6] + ROTL32(gx[5], 16) + ROTL32(gx[4], 16);
    ctx->x[7] = gx[7] + ROTL32(gx[6], 8) + gx[5];
    for (i = 0; i < 8; i++) ctx->x[i] = ctx->x[i] & 0xFFFFFFFF;
}

static void extract_keystream(RABBIT_ctx* ctx) {
    next_state(ctx);
    ctx->keystream[0] = ctx->x[0] ^ (ctx->x[5] >> 16) ^ (ctx->x[3] << 16);
    ctx->keystream[1] = ctx->x[2] ^ (ctx->x[7] >> 16) ^ (ctx->x[5] << 16);
    ctx->keystream[2] = ctx->x[4] ^ (ctx->x[1] >> 16) ^ (ctx->x[7] << 16);
    ctx->keystream[3] = ctx->x[6] ^ (ctx->x[3] >> 16) ^ (ctx->x[1] << 16);
}

void rabbit_keysetup(RABBIT_ctx* ctx, const u8* key, u32 keysize, u32 ivsize) {
    int i;
    u32 k[4];
    (void)ivsize;
    for (i = 0; i < 4; i++) {
        k[i] = ((u32)key[4*i+3] << 24) | ((u32)key[4*i+2] << 16) |
               ((u32)key[4*i+1] << 8) | (u32)key[4*i];
    }
    ctx->x[0] = k[0]; ctx->x[2] = k[1]; ctx->x[4] = k[2]; ctx->x[6] = k[3];
    ctx->x[1] = k[3] << 16; ctx->x[3] = k[0] << 16; ctx->x[5] = k[1] << 16; ctx->x[7] = k[2] << 16;
    ctx->c[0] = k[1] ^ (k[2] >> 16); ctx->c[1] = k[2] ^ (k[3] >> 16);
    ctx->c[2] = k[3] ^ (k[0] >> 16); ctx->c[3] = k[0] ^ (k[1] >> 16);
    ctx->c[4] = k[1]; ctx->c[5] = k[2]; ctx->c[6] = k[3]; ctx->c[7] = k[0];
    ctx->carry = 0; ctx->pos = 4;
    for (i = 0; i < 4; i++) next_state(ctx);
    for (i = 0; i < 4; i++) next_state(ctx);
}

void rabbit_ivsetup(RABBIT_ctx* ctx, const u8* iv) {
    int i;
    u32 iv0, iv1;
    iv0 = ((u32)iv[3] << 24) | ((u32)iv[2] << 16) | ((u32)iv[1] << 8) | (u32)iv[0];
    iv1 = ((u32)iv[7] << 24) | ((u32)iv[6] << 16) | ((u32)iv[5] << 8) | (u32)iv[4];
    ctx->c[0] ^= iv0; ctx->c[1] ^= iv1; ctx->c[2] ^= iv0 ^ (iv1 >> 16);
    ctx->c[3] ^= iv1 ^ (iv0 << 16); ctx->c[4] ^= iv0 ^ (iv1 >> 16);
    ctx->c[5] ^= iv1 ^ (iv0 << 16); ctx->c[6] ^= iv0; ctx->c[7] ^= iv1;
    for (i = 0; i < 4; i++) next_state(ctx);
    ctx->pos = 4;
}

void rabbit_encrypt_bytes(RABBIT_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen) {
    u32 i;
    u8* ks_bytes;
    for (i = 0; i < msglen; i++) {
        if (ctx->pos >= 4) { extract_keystream(ctx); ctx->pos = 0; }
        ks_bytes = (u8*)&ctx->keystream[ctx->pos / 4];
        if (plaintext) ciphertext[i] = plaintext[i] ^ ks_bytes[ctx->pos % 4];
        else ciphertext[i] = ks_bytes[ctx->pos % 4];
        ctx->pos++;
    }
}
