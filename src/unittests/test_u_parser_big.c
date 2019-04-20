/*
 * MIT License
 *
 * Copyright (c) 2019 Tomasz Szewczyk
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * SOFTWARE.
 *
 */

#include "unity.h"

#include "UParser.c"
#include "Stream.c"

#include <string.h>

#define BUFFER_SIZE 24

static Stream_T stream;
static uint8_t buffer[BUFFER_SIZE];

void setUp(void) {
    memset(buffer, 0, BUFFER_SIZE);
    Stream_Init(&stream, buffer, BUFFER_SIZE, BIG);
}

void test_serialize_u8(void) {
    uint8_t data1 = 0xAA;
    uint8_t data2 = 0xBB;
    uint8_t data3 = 0xCC;

    Status_T result1 = U8_Serialize(&data1, &stream);
    Status_T result2 = U8_Serialize(&data2, &stream);
    Status_T result3 = U8_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xAA, 0xBB, 0xCC};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_i8(void) {
    int8_t data1 = -54;
    int8_t data2 = -100;
    int8_t data3 = -128;

    Status_T result1 = I8_Serialize(&data1, &stream);
    Status_T result2 = I8_Serialize(&data2, &stream);
    Status_T result3 = I8_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xCA, 0x9C, 0x80};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_s8(void) {
    int8_t data1 = -54;
    int8_t data2 = -100;
    int8_t data3 = -127;

    Status_T result1 = S8_Serialize(&data1, &stream);
    Status_T result2 = S8_Serialize(&data2, &stream);
    Status_T result3 = S8_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xB6, 0xE4, 0xFF};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u8_bit(void) {
    uint8_t data1 = 0xAA;
    uint8_t data2 = 0xBB;
    uint8_t data3 = 0xCC;

    Status_T result1 = U8_Serialize(&data1, &stream);
    Status_T result2 = U8_SerializeBit(&data2, 4,&stream);
    Status_T result3 = U8_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xAA, 0xB0, 0xCC};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_i8_bit(void) {
    int8_t data1 = 7;
    int8_t data2 = -4;
    int8_t data3 = -2;

    Status_T result1 = I8_SerializeBit(&data1, 4,&stream);
    Status_T result2 = I8_SerializeBit(&data2, 4,&stream);
    Status_T result3 = I8_SerializeBit(&data3, 4,&stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0x7C, 0xE0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_s8_bit(void) {
    int8_t data1 = 7;
    int8_t data2 = -4;
    int8_t data3 = -2;

    Status_T result1 = S8_SerializeBit(&data1, 4,&stream);
    Status_T result2 = S8_SerializeBit(&data2, 4,&stream);
    Status_T result3 = S8_SerializeBit(&data3, 4,&stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0x7C, 0xA0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u8_bit2(void) {
    uint8_t data1 = 0x12;
    uint8_t data2 = 0x34;
    uint8_t data3 = 0x56;

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = U8_SerializeBit(&data2, 8, &stream);
    Status_T result3 = U8_SerializeBit(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0x23, 0x46};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u8_bit_overwidth(void) {
    uint8_t data1 = 0xAA;
    uint8_t data2 = 0xBB;
    uint8_t data3 = 0xCC;

    Status_T result1 = U8_Serialize(&data1, &stream);
    Status_T result2 = U8_SerializeBit(&data2, 16, &stream);
    Status_T result3 = U8_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xAA, 0x00, 0xBB, 0xCC};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u16(void) {
    uint16_t data1 = 0xAA11;
    uint16_t data2 = 0xBB22;
    uint16_t data3 = 0xCC33;

    Status_T result1 = U16_Serialize(&data1, &stream);
    Status_T result2 = U16_Serialize(&data2, &stream);
    Status_T result3 = U16_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0xAA, 0x11,
        0xBB, 0x22,
        0xCC, 0x33};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_i16(void) {
    int16_t data1 = -16895;
    int16_t data2 = -31674;
    int16_t data3 = -26586;

    Status_T result1 = I16_Serialize(&data1, &stream);
    Status_T result2 = I16_Serialize(&data2, &stream);
    Status_T result3 = I16_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0xBE, 0x01,
        0x84, 0x46,
        0x98, 0x26,
    };

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_s16(void) {
    int16_t data1 = -16895;
    int16_t data2 = -31674;
    int16_t data3 = -26586;

    Status_T result1 = S16_Serialize(&data1, &stream);
    Status_T result2 = S16_Serialize(&data2, &stream);
    Status_T result3 = S16_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0xC1, 0xFF,
        0xFB, 0xBA,
        0xE7, 0xDA,
    };

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u16_bit(void) {
    uint16_t data1 = 0xAA11;
    uint16_t data2 = 0xBB22;
    uint16_t data3 = 0xCC33;

    Status_T result1 = U16_SerializeBit(&data1, 12, &stream);
    Status_T result2 = U16_SerializeBit(&data2, 12, &stream);
    Status_T result3 = U16_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xA1, 0x1B, 0x22, 0xCC, 0x33};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_i16_bit(void) {
    int16_t data1 = 1256;
    int16_t data2 = -1789;
    int16_t data3 = -1132;

    Status_T result1 = I16_SerializeBit(&data1, 12, &stream);
    Status_T result2 = I16_SerializeBit(&data2, 12, &stream);
    Status_T result3 = I16_SerializeBit(&data3, 12, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0x4E, 0x89, 0x03, 0xB9, 0x40};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_s16_bit(void) {
    int16_t data1 = 1256;
    int16_t data2 = -1789;
    int16_t data3 = -1132;

    Status_T result1 = S16_SerializeBit(&data1, 12, &stream);
    Status_T result2 = S16_SerializeBit(&data2, 12, &stream);
    Status_T result3 = S16_SerializeBit(&data3, 12, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0x4E, 0x8E, 0xFD, 0xC6, 0xC0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u16_bit_overwidth(void) {
    uint16_t data1 = 0xAA11;
    uint16_t data2 = 0xBB22;
    uint16_t data3 = 0xCC33;

    Status_T result1 = U16_SerializeBit(&data1, 12, &stream);
    Status_T result2 = U16_SerializeBit(&data2, 32, &stream);
    Status_T result3 = U16_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {0xA1, 0x10, 0x00, 0x0B, 0xB2, 0x20, 0xCC, 0x33};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u32(void) {
    uint32_t data1 = 0x11223344;
    uint32_t data2 = 0x55667788;
    uint32_t data3 = 0xAABBCCDD;

    Status_T result1 = U32_Serialize(&data1, &stream);
    Status_T result2 = U32_Serialize(&data2, &stream);
    Status_T result3 = U32_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u32_bit(void) {
    uint32_t data1 = 0x11223344;
    uint32_t data2 = 0x55667788;
    uint32_t data3 = 0xAABBCCDD;

    Status_T result1 = U32_SerializeBit(&data1, 40, &stream);
    Status_T result2 = U32_SerializeBit(&data2, 24, &stream);
    Status_T result3 = U32_SerializeBit(&data3, 28, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x00, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xD0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u64(void) {
    uint64_t data1 = 0x1122334411223344;
    uint64_t data2 = 0x5566778855667788;
    uint64_t data3 = 0xAABBCCDDAABBCCDD;

    Status_T result1 = U64_Serialize(&data1, &stream);
    Status_T result2 = U64_Serialize(&data2, &stream);
    Status_T result3 = U64_Serialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88, 0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD, 0xAA, 0xBB, 0xCC, 0xDD};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_u64_bit(void) {
    uint64_t data1 = 0x1122334411223344;
    uint64_t data2 = 0x5566778855667788;
    uint64_t data3 = 0xAABBCCDDAABBCCDD;

    Status_T result1 = U64_SerializeBit(&data1, 72, &stream);
    Status_T result2 = U64_SerializeBit(&data2, 56, &stream);
    Status_T result3 = U64_SerializeBit(&data3, 60, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88, 0x55, 0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xDA, 0xAB, 0xBC, 0xCD, 0xD0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_float(void) {
    uint8_t data1 = 0x12;
    float   data2 = 12321.322265625f;

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Float_Serialize(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x20, 0x46, 0x40, 0x85, 0x4A};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_float_bit(void) {
    uint8_t data1 = 0x12;
    float   data2 = 12321.322265625f;

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Float_SerializeBit(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x24, 0x64, 0x08, 0x54, 0xA0};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_double(void) {
    uint8_t data1 = 0x12;
    double  data2 = 123123.54536344;

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Double_Serialize(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x20, 0x40, 0xFE, 0x0F, 0x38, 0xB9, 0xCF, 0x03, 0xB4};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_double_bit(void) {
    uint8_t data1 = 0x12;
    double  data2 = 123123.54536344;

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Double_SerializeBit(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x24, 0x0F, 0xE0, 0xF3, 0x8B, 0x9C, 0xF0, 0x3B, 0x40};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_array(void) {
    uint8_t data1   = 0x12;
    uint8_t data2[] = {0xAB, 0xCD, 0xEF};

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Array_Serialize(data2, sizeof(data2), &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x20, 0xAB, 0xCD, 0xEF};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_array_bit(void) {
    uint8_t data1   = 0x12;
    uint8_t data2[] = {0xAB, 0xCD, 0xEF};

    Status_T result1 = U8_SerializeBit(&data1, 4, &stream);
    Status_T result2 = Array_SerializeBit(data2, sizeof(data2), &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected[] = {0x2A, 0xBC, 0xDE, 0xF0};
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_size(void) {
    size_t data1 = 0x11223344;
    size_t data2 = 0x55667788;
    size_t data3 = 0xAABBCCDD;

    Status_T result1 = Size_Serialize(&data1, 4, &stream);
    Status_T result2 = Size_Serialize(&data2, 4, &stream);
    Status_T result3 = Size_Serialize(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_serialize_size_bit(void) {
    size_t data1 = 0x11223344;
    size_t data2 = 0x55667788;
    size_t data3 = 0xAABBCCDD;

    Status_T result1 = Size_SerializeBit(&data1, 40, &stream);
    Status_T result2 = Size_SerializeBit(&data2, 24, &stream);
    Status_T result3 = Size_SerializeBit(&data3, 28, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected[] = {
        0x00, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xD0};

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
}

void test_deserialize_u8(void) {
    uint8_t input[] = {0x12, 0x34, 0x56};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    Status_T result1 = U8_Deserialize(&data1, &stream);
    Status_T result2 = U8_Deserialize(&data2, &stream);
    Status_T result3 = U8_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected_data1 = 0x12;
    uint8_t expected_data2 = 0x34;
    uint8_t expected_data3 = 0x56;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_i8(void) {
    uint8_t input[] = {0x36, 0x9C, 0x80};
    memcpy(stream.p_buffer, input, sizeof(input));

    int8_t data1;
    int8_t data2;
    int8_t data3;

    Status_T result1 = I8_Deserialize(&data1, &stream);
    Status_T result2 = I8_Deserialize(&data2, &stream);
    Status_T result3 = I8_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int8_t expected_data1 = 54;
    int8_t expected_data2 = -100;
    int8_t expected_data3 = -128;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_s8(void) {
    uint8_t input[] = {0x36, 0xE4, 0xFF};
    memcpy(stream.p_buffer, input, sizeof(input));

    int8_t data1;
    int8_t data2;
    int8_t data3;

    Status_T result1 = S8_Deserialize(&data1, &stream);
    Status_T result2 = S8_Deserialize(&data2, &stream);
    Status_T result3 = S8_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int8_t expected_data1 = 54;
    int8_t expected_data2 = -100;
    int8_t expected_data3 = -127;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u8_bit(void) {
    uint8_t input[] = {0x12, 0x40, 0x56};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    Status_T result1 = U8_Deserialize(&data1, &stream);
    Status_T result2 = U8_DeserializeBit(&data2, 4,&stream);
    Status_T result3 = U8_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected_data1 = 0x12;
    uint8_t expected_data2 = 0x04;
    uint8_t expected_data3 = 0x56;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_i8_bit(void) {
    uint8_t input[] = {0x7C, 0xE0};
    memcpy(stream.p_buffer, input, sizeof(input));

    int8_t data1;
    int8_t data2;
    int8_t data3;

    Status_T result1 = I8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = I8_DeserializeBit(&data2, 4, &stream);
    Status_T result3 = I8_DeserializeBit(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int8_t expected_data1 = 7;
    int8_t expected_data2 = -4;
    int8_t expected_data3 = -2;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_s8_bit(void) {
    uint8_t input[] = {0x7C, 0xA0};
    memcpy(stream.p_buffer, input, sizeof(input));

    int8_t data1;
    int8_t data2;
    int8_t data3;

    Status_T result1 = S8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = S8_DeserializeBit(&data2, 4, &stream);
    Status_T result3 = S8_DeserializeBit(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int8_t expected_data1 = 7;
    int8_t expected_data2 = -4;
    int8_t expected_data3 = -2;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u8_bit2(void) {
    uint8_t input[] = {0x23, 0x46};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = U8_DeserializeBit(&data2, 8, &stream);
    Status_T result3 = U8_DeserializeBit(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected_data1 = 0x02;
    uint8_t expected_data2 = 0x34;
    uint8_t expected_data3 = 0x06;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u8_bit_overwidth(void) {
    uint8_t input[] = {0xAA, 0x00, 0xBB, 0xCC};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2;
    uint8_t data3;

    Status_T result1 = U8_Deserialize(&data1, &stream);
    Status_T result2 = U8_DeserializeBit(&data2, 16, &stream);
    Status_T result3 = U8_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint8_t expected_data1 = 0xAA;
    uint8_t expected_data2 = 0xBB;
    uint8_t expected_data3 = 0xCC;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u16(void) {
    uint8_t input[] = {
        0xAA, 0x11,
        0xBB, 0x22,
        0xCC, 0x33
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    uint16_t data1;
    uint16_t data2;
    uint16_t data3;

    Status_T result1 = U16_Deserialize(&data1, &stream);
    Status_T result2 = U16_Deserialize(&data2, &stream);
    Status_T result3 = U16_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint16_t expected_data1 = 0xAA11;
    uint16_t expected_data2 = 0xBB22;
    uint16_t expected_data3 = 0xCC33;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_i16(void) {
    uint8_t input[] = {
        0xBE, 0x01,
        0x84, 0x46,
        0x98, 0x26,
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    int16_t data1;
    int16_t data2;
    int16_t data3;

    Status_T result1 = I16_Deserialize(&data1, &stream);
    Status_T result2 = I16_Deserialize(&data2, &stream);
    Status_T result3 = I16_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int16_t expected_data1 = -16895;
    int16_t expected_data2 = -31674;
    int16_t expected_data3 = -26586;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_s16(void) {
    uint8_t input[] = {
        0xC1, 0xFF,
        0xFB, 0xBA,
        0xE7, 0xDA,
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    int16_t data1;
    int16_t data2;
    int16_t data3;

    Status_T result1 = S16_Deserialize(&data1, &stream);
    Status_T result2 = S16_Deserialize(&data2, &stream);
    Status_T result3 = S16_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int16_t expected_data1 = -16895;
    int16_t expected_data2 = -31674;
    int16_t expected_data3 = -26586;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u16_bit(void) {
    uint8_t input[] = {0xA1, 0x1B, 0x22, 0xCC, 0x33};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint16_t data1;
    uint16_t data2;
    uint16_t data3;

    Status_T result1 = U16_DeserializeBit(&data1, 12, &stream);
    Status_T result2 = U16_DeserializeBit(&data2, 12, &stream);
    Status_T result3 = U16_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint16_t expected_data1 = 0x0A11;
    uint16_t expected_data2 = 0x0B22;
    uint16_t expected_data3 = 0xCC33;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_i16_bit(void) {
    uint8_t input[] = {0x4E, 0x89, 0x03, 0xB9, 0x40};
    memcpy(stream.p_buffer, input, sizeof(input));

    int16_t data1;
    int16_t data2;
    int16_t data3;

    Status_T result1 = I16_DeserializeBit(&data1, 12, &stream);
    Status_T result2 = I16_DeserializeBit(&data2, 12, &stream);
    Status_T result3 = I16_DeserializeBit(&data3, 12, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int16_t expected_data1 = 1256;
    int16_t expected_data2 = -1789;
    int16_t expected_data3 = -1132;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_s16_bit(void) {
    uint8_t input[] = {0x4E, 0x8E, 0xFD, 0xC6, 0xC0};
    memcpy(stream.p_buffer, input, sizeof(input));

    int16_t data1;
    int16_t data2;
    int16_t data3;

    Status_T result1 = S16_DeserializeBit(&data1, 12, &stream);
    Status_T result2 = S16_DeserializeBit(&data2, 12, &stream);
    Status_T result3 = S16_DeserializeBit(&data3, 12, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    int16_t expected_data1 = 1256;
    int16_t expected_data2 = -1789;
    int16_t expected_data3 = -1132;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u16_bit_overwidth(void) {
    uint8_t input[] = {0xA1, 0x10, 0x00, 0x0B, 0xB2, 0x20, 0xCC, 0x33};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint16_t data1;
    uint16_t data2;
    uint16_t data3;

    Status_T result1 = U16_DeserializeBit(&data1, 12, &stream);
    Status_T result2 = U16_DeserializeBit(&data2, 32, &stream);
    Status_T result3 = U16_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint16_t expected_data1 = 0x0A11;
    uint16_t expected_data2 = 0xBB22;
    uint16_t expected_data3 = 0xCC33;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u32(void) {
    uint8_t input[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    uint32_t data1;
    uint32_t data2;
    uint32_t data3;

    Status_T result1 = U32_Deserialize(&data1, &stream);
    Status_T result2 = U32_Deserialize(&data2, &stream);
    Status_T result3 = U32_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint32_t expected_data1 = 0x11223344;
    uint32_t expected_data2 = 0x55667788;
    uint32_t expected_data3 = 0xAABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u32_bit(void) {
    uint8_t input[] = {
        0x00, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xD0
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    uint32_t data1;
    uint32_t data2;
    uint32_t data3;

    Status_T result1 = U32_DeserializeBit(&data1, 40, &stream);
    Status_T result2 = U32_DeserializeBit(&data2, 24, &stream);
    Status_T result3 = U32_DeserializeBit(&data3, 28, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint32_t expected_data1 = 0x11223344;
    uint32_t expected_data2 = 0x00667788;
    uint32_t expected_data3 = 0x0ABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u64(void) {
    uint8_t input[] = {
        0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88, 0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD, 0xAA, 0xBB, 0xCC, 0xDD
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    uint64_t data1;
    uint64_t data2;
    uint64_t data3;

    Status_T result1 = U64_Deserialize(&data1, &stream);
    Status_T result2 = U64_Deserialize(&data2, &stream);
    Status_T result3 = U64_Deserialize(&data3, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint64_t expected_data1 = 0x1122334411223344;
    uint64_t expected_data2 = 0x5566778855667788;
    uint64_t expected_data3 = 0xAABBCCDDAABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_u64_bit(void) {
    uint8_t input[] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88, 0x55, 0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xDA, 0xAB, 0xBC, 0xCD, 0xD0
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    uint64_t data1;
    uint64_t data2;
    uint64_t data3;

    Status_T result1 = U64_DeserializeBit(&data1, 72, &stream);
    Status_T result2 = U64_DeserializeBit(&data2, 56, &stream);
    Status_T result3 = U64_DeserializeBit(&data3, 60, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    uint64_t expected_data1 = 0x1122334411223344;
    uint64_t expected_data2 = 0x0066778855667788;
    uint64_t expected_data3 = 0x0ABBCCDDAABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_float(void) {
    uint8_t input[] = {0x20, 0x46, 0x40, 0x85, 0x4A};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    float   data2;

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Float_Deserialize(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1 = 0x02;
    float   expected_data2 = 12321.322265625f;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
}

void test_deserialize_float_bit(void) {
    uint8_t input[] = {0x24, 0x64, 0x08, 0x54, 0xA0};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    float   data2;

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Float_DeserializeBit(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1 = 0x02;
    float   expected_data2 = 12321.322265625f;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
}

void test_deserialize_double(void) {
    uint8_t input[] = {0x20, 0x40, 0xFE, 0x0F, 0x38, 0xB9, 0xCF, 0x03, 0xB4};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    double  data2;

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Double_Deserialize(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1 = 0x02;
    double  expected_data2 = 123123.54536344;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
}

void test_deserialize_double_bit(void) {
    uint8_t input[] = {0x24, 0x0F, 0xE0, 0xF3, 0x8B, 0x9C, 0xF0, 0x3B, 0x40};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    double  data2;

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Double_DeserializeBit(&data2, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1 = 0x02;
    double  expected_data2 = 123123.54536344;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
}

void test_deserialize_array(void) {
    uint8_t input[] = {0x20, 0xAB, 0xCD, 0xEF};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2[3];

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Array_Deserialize(data2, sizeof(data2), &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1   = 0x02;
    uint8_t expected_data2[] = {0xAB, 0xCD, 0xEF};

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data2, data2, sizeof(expected_data2));
}

void test_deserialize_array_bit(void) {
    uint8_t input[] = {0x2A, 0xBC, 0xDE, 0xF0};
    memcpy(stream.p_buffer, input, sizeof(input));

    uint8_t data1;
    uint8_t data2[3];

    Status_T result1 = U8_DeserializeBit(&data1, 4, &stream);
    Status_T result2 = Array_DeserializeBit(data2, sizeof(data2), &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);

    uint8_t expected_data1   = 0x02;
    uint8_t expected_data2[] = {0xAB, 0xCD, 0xEF};

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_data2, data2, sizeof(expected_data2));
}

void test_deserialize_size(void) {
    uint8_t input[] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0xAA, 0xBB, 0xCC, 0xDD
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    size_t data1;
    size_t data2;
    size_t data3;

    Status_T result1 = Size_Deserialize(&data1, 4, &stream);
    Status_T result2 = Size_Deserialize(&data2, 4, &stream);
    Status_T result3 = Size_Deserialize(&data3, 4, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    size_t expected_data1 = 0x11223344;
    size_t expected_data2 = 0x55667788;
    size_t expected_data3 = 0xAABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}

void test_deserialize_size_bit(void) {
    uint8_t input[] = {
        0x00, 0x11, 0x22, 0x33, 0x44,
        0x66, 0x77, 0x88,
        0xAB, 0xBC, 0xCD, 0xD0
    };
    memcpy(stream.p_buffer, input, sizeof(input));

    size_t data1;
    size_t data2;
    size_t data3;

    Status_T result1 = Size_DeserializeBit(&data1, 40, &stream);
    Status_T result2 = Size_DeserializeBit(&data2, 24, &stream);
    Status_T result3 = Size_DeserializeBit(&data3, 28, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);

    size_t expected_data1 = 0x11223344;
    size_t expected_data2 = 0x00667788;
    size_t expected_data3 = 0x0ABBCCDD;

    TEST_ASSERT_EQUAL(expected_data1, data1);
    TEST_ASSERT_EQUAL(expected_data2, data2);
    TEST_ASSERT_EQUAL(expected_data3, data3);
}
