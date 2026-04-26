#include "salsa20.h"
#include <string.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) \
    b ^= ROTL(a + d, 7); \
    c ^= ROTL(b + a, 9); \
    d ^= ROTL(c + b, 13); \
    a ^= ROTL(d + c, 18);

static const uint8_t sigma[16] = {
    'e', 'x', 'p', 'a', 'n', 'd', ' ', '3', '2', '-', 'b', 'y', 't', 'e', ' ', 'k'
};

void Salsa20_init(void) {}

void Salsa20_keysetup(Salsa20_ctx* ctx, const uint8_t* key, size_t keybits) {
    const uint8_t* constants = sigma;
    
    ctx->input[0] = ((uint32_t*)constants)[0];
    ctx->input[1] = ((uint32_t*)key)[0];
    ctx->input[2] = ((uint32_t*)key)[1];
    ctx->input[3] = ((uint32_t*)key)[2];
    ctx->input[4] = ((uint32_t*)key)[3];
    ctx->input[5] = ((uint32_t*)constants)[1];
    ctx->input[6] = 0;  // будет установлено в ivsetup
    ctx->input[7] = 0;  // будет установлено в ivsetup
    ctx->input[8] = 0;  // счётчик (младшая часть)
    ctx->input[9] = 0;  // счётчик (старшая часть)
    ctx->input[10] = ((uint32_t*)constants)[2];
    ctx->input[11] = 0;  // не используется
    ctx->input[12] = 0;  // не используется
    ctx->input[13] = 0;  // не используется
    ctx->input[14] = 0;  // не используется
    ctx->input[15] = ((uint32_t*)constants)[3];
    ctx->pos = 64;
}

void Salsa20_ivsetup(Salsa20_ctx* ctx, const uint8_t* iv) {
    ctx->input[6] = ((uint32_t*)iv)[0];
    ctx->input[7] = ((uint32_t*)iv)[1];
    ctx->input[8] = 0;  // Сбрасываем счётчик при установке IV
    ctx->input[9] = 0;
    ctx->pos = 64;
}

static void salsa20_core(uint32_t output[16], const uint32_t input[16]) {
    uint32_t x[16];
    int i;
    
    for (i = 0; i < 16; ++i) x[i] = input[i];
    
    for (i = 20; i > 0; i -= 2) {
        QR(x[0], x[4], x[ 8], x[12]);
        QR(x[5], x[9], x[13], x[ 1]);
        QR(x[10], x[14], x[2], x[ 6]);
        QR(x[15], x[3], x[7], x[11]);
        
        QR(x[0], x[1], x[ 2], x[ 3]);
        QR(x[5], x[6], x[ 7], x[ 4]);
        QR(x[10], x[11], x[ 8], x[ 9]);
        QR(x[15], x[12], x[13], x[14]);
    }
    
    for (i = 0; i < 16; ++i) output[i] = x[i] + input[i];
}

static void Salsa20_generate_keystream(Salsa20_ctx* ctx) {
    uint32_t output[16];
    salsa20_core(output, ctx->input);
    
    // Копируем в keystream как массив байт (little-endian)
    for (size_t i = 0; i < 16; i++) {
        ctx->keystream[i * 4 + 0] = (output[i] >> 0) & 0xFF;
        ctx->keystream[i * 4 + 1] = (output[i] >> 8) & 0xFF;
        ctx->keystream[i * 4 + 2] = (output[i] >> 16) & 0xFF;
        ctx->keystream[i * 4 + 3] = (output[i] >> 24) & 0xFF;
    }
    
    // Инкрементируем счётчик
    ctx->input[8]++;
    if (ctx->input[8] == 0) {
        ctx->input[9]++;
    }
    
    ctx->pos = 0;
}

void Salsa20_encrypt_bytes(Salsa20_ctx* ctx, const uint8_t* plaintext, uint8_t* ciphertext, size_t msglen) {
    for (size_t i = 0; i < msglen; i++) {
        if (ctx->pos >= 64) {
            Salsa20_generate_keystream(ctx);
        }
        ciphertext[i] = plaintext[i] ^ ctx->keystream[ctx->pos++];
    }
}