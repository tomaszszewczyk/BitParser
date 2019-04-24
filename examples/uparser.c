#include <stdio.h>
#include <stdint.h>

#include "UParser.h"

int main() {
    uint32_t a = 12654;
    int32_t  b = -13543;
    float    c = 0.5;

    uint8_t msg[13] = {0};
    Stream_T stream;

    Stream_Init(&stream, msg, sizeof(msg), BIG);
    U32_SerializeBit(&a, 24, &stream);
    I32_SerializeBit(&b, 24, &stream);
    S32_SerializeBit(&b, 24, &stream);
    Float_SerializeBit(&c, &stream);

    for(size_t i = 0; i < sizeof(msg); i++)
        printf("0x%02X ", msg[i]);
    printf("\n");
}