#include "hc128.h"
#include <string.h>

#define ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

static u32 f1(u32 x) { return ROTL32(x, 7) ^ ROTL32(x, 18) ^ (x >> 3); }
static u32 f2(u32 x) { return ROTL32(x, 17) ^ ROTL32(x, 19) ^ (x >> 10); }
static u32 g1(u32 x, u32 y, u32 z) { return (ROTL32(x, 10) ^ ROTL32(z, 23)) + ROTL32(y, 8); }
static u32 g2(u32 x, u32 y, u32 z) { return (ROTL32(x, 12) ^ ROTL32(z, 18)) + ROTL32(y, 17); }
static u32 h1(u32 x) { return (x >> 16) & 0xFFFF; }
static u32 h2(u32 x) { return (x >> 16) & 0xFFFF; }

void hc128_keysetup(HC128_ctx* ctx, const u8* key, u32 keysize, u32 ivsize) {
    int i;
    u32 w[1280];
    (void)ivsize;
    
    for (i = 0; i < 8; i++) ctx->key[i] = 0;
    for (i = 0; i < keysize / 32; i++) {
        ctx->key[i] = ((u32)key[4*i+3] << 24) | ((u32)key[4*i+2] << 16) |
                      ((u32)key[4*i+1] << 8) | (u32)key[4*i];
    }
    
    for (i = 0; i < 8; i++) w[i] = ctx->key[i];
    for (i = 8; i < 1280; i++) w[i] = (w[i-3] >> 3) ^ (w[i-7] >> 23) ^ (w[i-8] >> 8) ^ (w[i-10] >> 17);
    
    for (i = 0; i < 512; i++) { ctx->P[i] = w[i+256]; ctx->Q[i] = w[i+768]; }
    for (i = 0; i < 512; i++) {
        ctx->P[i] = (ctx->P[i] + ctx->Q[(ctx->P[i] >> 10) & 0x1FF]) & 0xFFFFFFFF;
        ctx->Q[i] = (ctx->Q[i] + ctx->P[(ctx->Q[i] >> 10) & 0x1FF]) & 0xFFFFFFFF;
    }
    ctx->counter = 0;
    ctx->pos = 4;
}

void hc128_ivsetup(HC128_ctx* ctx, const u8* iv) {
    int i;
    u32 w[1280];
    for (i = 0; i < 8; i++) ctx->iv[i] = 0;
    for (i = 0; i < 4; i++) {
        ctx->iv[i] = ((u32)iv[4*i+3] << 24) | ((u32)iv[4*i+2] << 16) |
                     ((u32)iv[4*i+1] << 8) | (u32)iv[4*i];
    }
    for (i = 0; i < 8; i++) { w[i] = ctx->key[i]; w[i+8] = ctx->iv[i]; }
    for (i = 16; i < 1280; i++) w[i] = (w[i-3] >> 3) ^ (w[i-7] >> 23) ^ (w[i-8] >> 8) ^ (w[i-10] >> 17);
    for (i = 0; i < 512; i++) { ctx->P[i] = w[i+256]; ctx->Q[i] = w[i+768]; }
    for (i = 0; i < 512; i++) {
        ctx->P[i] = (ctx->P[i] + ctx->Q[(ctx->P[i] >> 10) & 0x1FF]) & 0xFFFFFFFF;
        ctx->Q[i] = (ctx->Q[i] + ctx->P[(ctx->Q[i] >> 10) & 0x1FF]) & 0xFFFFFFFF;
    }
    ctx->counter = 0;
    ctx->pos = 4;
}

static void generate_keystream(HC128_ctx* ctx) {
    int i;
    u32 p, q;
    for (i = 0; i < 4; i++) {
        p = ctx->P[ctx->counter & 0x1FF];
        q = ctx->Q[(ctx->counter >> 10) & 0x1FF];
        ctx->keystream[i] = (g1(ctx->P[(ctx->counter - 3) & 0x1FF], ctx->P[(ctx->counter - 10) & 0x1FF], p) ^ h1(ctx->Q[(p >> 10) & 0x1FF])) & 0xFFFFFFFF;
        ctx->keystream[i+4] = (g2(ctx->Q[(ctx->counter - 3) & 0x1FF], ctx->Q[(ctx->counter - 10) & 0x1FF], q) ^ h2(ctx->P[(q >> 10) & 0x1FF])) & 0xFFFFFFFF;
        ctx->counter = (ctx->counter + 1) & 0x3FF;
    }
}

void hc128_encrypt_bytes(HC128_ctx* ctx, const u8* plaintext, u8* ciphertext, u32 msglen) {
    u32 i;
    u8* ks_bytes;
    for (i = 0; i < msglen; i++) {
        if (ctx->pos >= 4) { generate_keystream(ctx); ctx->pos = 0; }
        ks_bytes = (u8*)&ctx->keystream[ctx->pos / 4];
        if (plaintext) ciphertext[i] = plaintext[i] ^ ks_bytes[ctx->pos % 4];
        else ciphertext[i] = ks_bytes[ctx->pos % 4];
        ctx->pos++;
    }
}
