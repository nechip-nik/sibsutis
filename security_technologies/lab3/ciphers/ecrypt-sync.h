#ifndef ECRYPT_SYNC_H
#define ECRYPT_SYNC_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint32_t u32;

#define ECRYPT_MAXKEYSIZE 32
#define ECRYPT_MAXIVSIZE 16

void ECRYPT_init(void);

#endif