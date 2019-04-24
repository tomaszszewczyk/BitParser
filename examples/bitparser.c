#include <stdio.h>
#include <stdint.h>

#include "BitParser.h"

typedef struct {
    uint32_t a;
    int32_t  b;
    float    c;
} MyMsg_T;

static const BitField_T my_msg_desc[] = {
    BIT_FIELD_U32(24, MyMsg_T, a),
    BIT_FIELD_I32(24, MyMsg_T, b),
    BIT_FIELD_S32(24, MyMsg_T, b),
    BIT_FIELD_FLOAT(MyMsg_T, c),
};

int main() {
    MyMsg_T data = {.a = 12654, .b = -13543, .c = 0.5};
    uint8_t msg[13] = {0};

    Stream_T stream;
    Stream_Init(&stream, msg, sizeof(msg), BIG);

    Status_T ret = BitParser_Serialize(my_msg_desc, ARRAY_LEN(my_msg_desc), &data, &stream);
    if(ret != STATUS_SUCCESS)
        return 1;

    for(size_t i = 0; i < sizeof(msg); i++)
        printf("0x%02X ", msg[i]);
    printf("\n");
}