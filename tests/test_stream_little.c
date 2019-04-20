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

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "Stream.h"

#define BUFFER_SIZE 8

static Stream_T stream;
static uint8_t buffer[BUFFER_SIZE];

void setUp(void) {
    memset(buffer, 0, BUFFER_SIZE);
    Stream_Init(&stream, buffer, BUFFER_SIZE, LITTLE);
}

void test_size(void) {
    size_t resulting_size = Stream_GetSize(&stream);
    TEST_ASSERT_EQUAL(BUFFER_SIZE, resulting_size);
}

void test_size_in_bits(void) {
    size_t resulting_size = Stream_GetSizeBits(&stream);
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8, resulting_size);
}

void test_seek(void) {
    uint8_t new_data[] = {0x01, 0x02, 0x03};
    uint8_t read_data[sizeof(new_data)];

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result1 = Stream_Seek(&stream, 1);
    Status_T result2 = Stream_Read(&stream, read_data, sizeof(new_data)-1);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_data+1, read_data, sizeof(new_data)-1);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(new_data), Stream_Tell(&stream));
}

void test_seek_bits(void) {
    uint8_t new_data[] = {0xAB};
    uint8_t expected   = 0x0A;
    uint8_t read_data  = 0;

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result1 = Stream_SeekBit(&stream, 4);
    Status_T result2 = Stream_ReadBit(&stream, &read_data, 4);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(expected, read_data);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
}

void test_seek_bits_in_byte(void) {
    uint8_t new_data[] = {0xFF, 0xAB};
    uint8_t expected   = 0x0A;
    uint8_t read_data  = 0;

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result1 = Stream_Seek(&stream, 1);
    Status_T result2 = Stream_SeekBitInByte(&stream, 4);
    Status_T result3 = Stream_ReadBit(&stream, &read_data, 4);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result3);
    TEST_ASSERT_EQUAL(expected, read_data);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(2, Stream_Tell(&stream));
}

void test_read(void) {
    uint8_t new_data[] = {0x01, 0x02, 0x03};
    uint8_t read_data[sizeof(new_data)];

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result = Stream_Read(&stream, read_data, sizeof(new_data));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_data, read_data, sizeof(new_data));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(new_data), Stream_Tell(&stream));
}

void test_read_two_times(void) {
    uint8_t new_data[] = {0x01, 0x02, 0x03, 0xF1, 0xF2, 0xF3};
    memcpy(buffer, new_data, sizeof(new_data));

    uint8_t read_data1[4];
    uint8_t read_data2[2];

    Status_T result1 = Stream_Read(&stream, read_data1, sizeof(read_data1));
    Status_T result2 = Stream_Read(&stream, read_data2, sizeof(read_data2));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(read_data1, new_data, sizeof(read_data1));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(read_data2, new_data + sizeof(read_data1), sizeof(read_data2));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(read_data1) - sizeof(read_data2), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(read_data1) + sizeof(read_data2), Stream_Tell(&stream));
}

void test_read_too_much(void) {
    uint8_t read[BUFFER_SIZE + 1];
    Status_T result = Stream_Read(&stream, read, sizeof(read));
    TEST_ASSERT_EQUAL(ERROR_STREAM_TOO_SHORT, result);
}

void test_read_half_byte(void) {
    uint8_t new_data[] = {0x0F};
    uint8_t expected   = 0x0F;
    uint8_t read_data  = 0;

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result = Stream_ReadBit(&stream, &read_data, 4);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL(expected, read_data);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(0, Stream_Tell(&stream));
}

void test_read_half_byte_and_byte(void) {
    uint8_t new_data[] = {0xAB, 0xCD};
    uint8_t expected1  = 0x0B;
    uint8_t read_data1 = 0;
    uint8_t expected2  = 0xDA;
    uint8_t read_data2 = 0;

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result1 = Stream_ReadBit(&stream, &read_data1, 4);
    Status_T result2 = Stream_ReadBit(&stream, &read_data2, 8);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(expected1, read_data1);
    TEST_ASSERT_EQUAL(expected2, read_data2);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
}

void test_read_half_byte_and_two_bytes(void) {
    uint8_t  new_data[] = {0x12, 0x34, 0x56};
    uint8_t  expected1  = 0x02;
    uint8_t  read_data1 = 0;
    uint8_t  expected2[] = {0x41, 0x63};
    uint8_t  read_data2[sizeof(expected2)];

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result1 = Stream_ReadBit(&stream, &read_data1, 4);
    Status_T result2 = Stream_ReadBit(&stream, (uint8_t *) &read_data2, 16);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL(expected1, read_data1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected2, read_data2, sizeof(expected2));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(2, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - (20), Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(4, Stream_GetLeftBitsInByte(&stream));
}

void test_read_one_and_some(void) {
    uint8_t new_data[] = {0x34, 0xF2};
    uint8_t expected[] = {0x34, 0x12};
    size_t  bit_count  = 13;
    uint8_t output[sizeof(expected)];

    memcpy(buffer, new_data, sizeof(new_data));
    Status_T result = Stream_ReadBit(&stream, output, bit_count);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(expected), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - bit_count, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(3, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count, Stream_TellBit(&stream));
}

void test_write(void) {
    uint8_t new_data[] = {0x01, 0x02, 0x03};
    Status_T result = Stream_Write(&stream, new_data, sizeof(new_data));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_data, buffer, sizeof(new_data));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(new_data), Stream_Tell(&stream));
}

void test_write_two_times(void) {
    uint8_t new_data[]     = {0x01, 0x02, 0x03};
    uint8_t another_data[] = {0xF1, 0xF2, 0xF3};
    Status_T result1 = Stream_Write(&stream, new_data, sizeof(new_data));
    Status_T result2 = Stream_Write(&stream, another_data, sizeof(another_data));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(new_data, buffer, sizeof(new_data));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(another_data, buffer + sizeof(new_data), sizeof(another_data));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data) - sizeof(another_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(new_data) + sizeof(another_data), Stream_Tell(&stream));
}

void test_write_too_much(void) {
    uint8_t first_data[]  = {0x01, 0x02, 0x03, 0x04, 0x05};
    uint8_t second_data[] = {0xF1, 0xF2, 0xF3, 0xF4, 0xF5};
    Status_T result1 = Stream_Write(&stream, first_data, sizeof(first_data));
    Status_T result2 = Stream_Write(&stream, second_data, sizeof(second_data));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(ERROR_STREAM_TOO_SHORT, result2);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(first_data, buffer, sizeof(first_data));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(first_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(first_data), Stream_Tell(&stream));
}

void test_write_half_a_byte(void) {
    uint8_t new_data[] = {0xAB};
    uint8_t expected[] = {0x0B};
    size_t  bit_count  = 4;
    Status_T result = Stream_WriteBit(&stream, new_data, bit_count);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(0, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - bit_count, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(4, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count, Stream_TellBit(&stream));
}

void test_write_two_bits(void) {
    uint8_t new_data[] = {0b10101010};
    uint8_t expected[] = {0b00000010};
    size_t  bit_count  = 2;
    Status_T result = Stream_WriteBit(&stream, new_data, bit_count);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(0, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - bit_count, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(6, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count, Stream_TellBit(&stream));
}

void test_write_one_and_a_half_byte(void) {
    uint8_t new_data[] = {0xBA, 0xDC};
    uint8_t expected[] = {0xBA, 0x0C};
    size_t  bit_count  = 12;
    Status_T result = Stream_WriteBit(&stream, new_data, bit_count);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - bit_count, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(4, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count, Stream_TellBit(&stream));
}

void test_write_one_and_some(void) {
    uint8_t new_data[] = {0x34, 0xF2};
    uint8_t expected[] = {0x34, 0x12};
    size_t  bit_count  = 13;
    Status_T result = Stream_WriteBit(&stream, new_data, bit_count);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(new_data), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - bit_count, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(3, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count, Stream_TellBit(&stream));
}

void test_write_half_a_byte_and_a_byte(void) {
    uint8_t new_data1[] = {0xFF};
    uint8_t new_data2[] = {0xBB};
    uint8_t expected[]  = {0xBF, 0x0B};

    size_t bit_count1 = 4;
    size_t bit_count2 = 8;

    Status_T result1 = Stream_WriteBit(&stream, new_data1, bit_count1);
    Status_T result2 = Stream_WriteBit(&stream, new_data2, bit_count2);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(expected), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - (bit_count1 + bit_count2), Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(4, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count1 + bit_count2, Stream_TellBit(&stream));
}

void test_write_some_and_a_byte(void) {
    uint8_t new_data1[] = {0xFF};
    uint8_t new_data2[] = {0xBB};
    uint8_t expected[]  = {0xEF, 0x02};

    size_t bit_count1 = 2;
    size_t bit_count2 = 8;

    Status_T result1 = Stream_WriteBit(&stream, new_data1, bit_count1);
    Status_T result2 = Stream_WriteBit(&stream, new_data2, bit_count2);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(expected), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(1, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - (bit_count1 + bit_count2), Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(6, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count1 + bit_count2, Stream_TellBit(&stream));
}

void test_write_half_a_byte_and_two_bytes(void) {
    uint8_t new_data1[] = {0xFF};
    uint8_t new_data2[] = {0xBB, 0xAA};
    uint8_t expected[]  = {0xBF, 0xAB, 0x0A};

    size_t bit_count1 = 4;
    size_t bit_count2 = 16;

    Status_T result1 = Stream_WriteBit(&stream, new_data1, bit_count1);
    Status_T result2 = Stream_WriteBit(&stream, new_data2, bit_count2);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(expected), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(2, Stream_Tell(&stream));
    TEST_ASSERT_EQUAL(BUFFER_SIZE * 8 - (bit_count1 + bit_count2), Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(4, Stream_GetLeftBitsInByte(&stream));
    TEST_ASSERT_EQUAL(bit_count1 + bit_count2, Stream_TellBit(&stream));
}

void test_write_half_a_byte_and_regular_write(void) {
    uint8_t new_data1[] = {0xFF};
    uint8_t new_data2[] = {0xBB};
    uint8_t expected[] = {0x0F, 0xBB};
    size_t  bit_count  = 4;
    Status_T result1 = Stream_WriteBit(&stream, new_data1, bit_count);
    Status_T result2 = Stream_Write(&stream, new_data2, sizeof(new_data2));

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result1);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, buffer, sizeof(expected));
    TEST_ASSERT_EQUAL(BUFFER_SIZE - sizeof(expected), Stream_GetLeft(&stream));
    TEST_ASSERT_EQUAL(sizeof(expected), Stream_Tell(&stream));
    TEST_ASSERT_EQUAL((BUFFER_SIZE - sizeof(expected)) * 8, Stream_GetLeftBits(&stream));
    TEST_ASSERT_EQUAL(sizeof(expected) * 8, Stream_TellBit(&stream));
}
