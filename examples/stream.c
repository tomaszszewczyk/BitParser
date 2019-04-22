#include <stdio.h>
#include <stdint.h>

#include "Stream.h"

int main() {
    uint8_t data1 = 0xAA, data2 = 0xBB, data3 = 0xCC, data4 = 0xDD;
    uint8_t message[3] = {0};
    Stream_T stream;

    Stream_Init(&stream, message, sizeof(message), BIG);
    Stream_WriteBit(&stream, &data1, 4);
    Stream_SeekBit(&stream, 8);
    Stream_WriteBit(&stream, &data2, 4);
    Stream_WriteBit(&stream, &data3, 4);
    Stream_WriteBit(&stream, &data4, 4);

    printf("0x%02X 0x%02X 0x%02X\n", message[0], message[1], message[2]);
}