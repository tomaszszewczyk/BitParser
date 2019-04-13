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

#include <string.h>

#include "Stream.h"
#include "BitParserError.h"

#define BYTE_INDEX(x)  ((x) / BITS_IN_BYTE)
#define BIT_IN_BYTE(x) ((x) % BITS_IN_BYTE)

/**
 * Generate 8-bit mask. Will return byte with bits from start to stop included set.
 *
 * @param start     First set bit index.
 * @param stop      Last set bit index.
 * @return          Created mask.
 */
static uint8_t Stream_GetMask(size_t start, size_t stop);

/**
 * Write bit_count bits of value into p_dst at index bit.
 *
 * @param p_dst     Pointer to dst byte
 * @param value     Value to write
 * @param index     Destination byte index (shall be lower than 8)
 * @param bit_count Number of bits to write
 */
static void Stream_WritePartByte(uint8_t *p_dst, uint8_t value, size_t index, size_t bit_count, Stream_Mode_T mode);

/**
 * Read bit_count bits of value from index bit.
 *
 * @param value     Byte to read from.
 * @param index     Index to read from.
 * @param bit_count Bits to read.
 * @return          Read data.
 */
static uint8_t Stream_ReadPartByte(uint8_t value, size_t index, size_t bit_count, Stream_Mode_T mode);

void Stream_Init(Stream_T * p_self, uint8_t * p_buffer, size_t len, Stream_Mode_T mode) {
    ASSERT(p_self != NULL);
    ASSERT(p_self != NULL);
    ASSERT(len != 0);
    ASSERT(mode == LITTLE || mode == BIG);

    p_self->p_buffer  = p_buffer;
    p_self->bit_len   = len * BITS_IN_BYTE;
    p_self->bit_index = 0;
    p_self->mode      = mode;
}

Stream_Mode_T Stream_GetMode(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return p_self->mode;
}

Status_T Stream_SetMode(Stream_T * p_self, Stream_Mode_T mode) {
    ASSERT(p_self != NULL);
    ASSERT(mode == LITTLE || mode == BIG);

    if(p_self->mode == mode)
        return STATUS_SUCCESS;

    if(Stream_TellBitInByte(p_self) != 0)
        return ERROR_STREAM_NOT_ALIGNED;

    p_self->mode = mode;
    return STATUS_SUCCESS;
}

size_t Stream_GetSize(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return p_self->bit_len / BITS_IN_BYTE;
}

size_t Stream_GetSizeBits(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return p_self->bit_len;
}

size_t Stream_GetLeft(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return (p_self->bit_len - p_self->bit_index) / BITS_IN_BYTE;
}

size_t Stream_GetLeftBits(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return p_self->bit_len - p_self->bit_index;
}

size_t Stream_GetLeftBitsInByte(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return Stream_GetLeftBits(p_self) % BITS_IN_BYTE;
}

Status_T Stream_Seek(Stream_T * p_self, size_t index) {
    ASSERT(p_self != NULL);

    if(index >= Stream_GetSize(p_self))
        return ERROR_STREAM_TOO_SHORT;

    p_self->bit_index = index * BITS_IN_BYTE;

    return STATUS_SUCCESS;
}

Status_T Stream_SeekBit(Stream_T * p_self, size_t bit_index) {
    ASSERT(p_self != NULL);

    if(bit_index > Stream_GetSizeBits(p_self))
        return ERROR_STREAM_TOO_SHORT;

    p_self->bit_index = bit_index;

    return STATUS_SUCCESS;
}

Status_T Stream_SeekBitInByte(Stream_T * p_self, size_t bit_index) {
    ASSERT(p_self != NULL);

    p_self->bit_index = Stream_Tell(p_self) * BITS_IN_BYTE + bit_index;

    return STATUS_SUCCESS;
}

size_t Stream_Tell(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return BYTE_INDEX(p_self->bit_index);
}

size_t Stream_TellBit(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return p_self->bit_index;
}

size_t Stream_TellBitInByte(Stream_T * p_self) {
    ASSERT(p_self != NULL);

    return BIT_IN_BYTE(p_self->bit_index);
}

void Stream_Align(Stream_T * p_self) {
    ASSERT(p_self);

    size_t bit_in_byte = Stream_TellBitInByte(p_self);
    p_self->bit_index += bit_in_byte != 0 ? BITS_IN_BYTE - bit_in_byte : 0;
}

Status_T Stream_Write(Stream_T * p_self, uint8_t * p_data, size_t len) {
    ASSERT(p_self != NULL);
    ASSERT(p_data != NULL);

    if(Stream_GetLeft(p_self) < len)
        return ERROR_STREAM_TOO_SHORT;

    Stream_Align(p_self);

    memcpy(p_self->p_buffer + Stream_Tell(p_self), p_data, len);
    p_self->bit_index += len * BITS_IN_BYTE;

    return STATUS_SUCCESS;
}

Status_T Stream_WriteBit(Stream_T * p_self, uint8_t * p_data, size_t bit_len) {
    ASSERT(p_self != NULL);
    ASSERT(p_data != NULL);

    if(Stream_GetLeftBits(p_self) < bit_len)
        return ERROR_STREAM_TOO_SHORT;

    size_t offset = 0;
    if(bit_len % BITS_IN_BYTE != 0 && p_self->mode == BIG)
        offset += BITS_IN_BYTE - (bit_len % BITS_IN_BYTE);

    for(size_t foreign_index = offset; foreign_index < bit_len + offset;) {
        size_t    foreign_size = MIN(BITS_IN_BYTE - BIT_IN_BYTE(foreign_index), (bit_len + offset) - foreign_index);
        size_t    own_size     = MIN(BITS_IN_BYTE - Stream_TellBitInByte(p_self), Stream_GetLeftBits(p_self));
        size_t    move_size    = MIN(foreign_size, own_size);
        uint8_t * input        = p_data + BYTE_INDEX(foreign_index);
        uint8_t * output       = p_self->p_buffer + Stream_Tell(p_self);

        uint8_t data_to_write = Stream_ReadPartByte(*input, foreign_index, move_size, p_self->mode);
        Stream_WritePartByte(output, data_to_write, p_self->bit_index, move_size, p_self->mode);

        p_self->bit_index += move_size;
        foreign_index     += move_size;
    }

    return STATUS_SUCCESS;
}

Status_T Stream_Read(Stream_T * p_self, uint8_t * p_data, size_t len) {
    ASSERT(p_self != NULL);
    ASSERT(p_data != NULL);

    if(Stream_GetLeft(p_self) < len)
        return ERROR_STREAM_TOO_SHORT;

    Stream_Align(p_self);

    memcpy(p_data, p_self->p_buffer + Stream_Tell(p_self), len);
    p_self->bit_index += len * BITS_IN_BYTE;

    return STATUS_SUCCESS;
}

Status_T Stream_ReadBit(Stream_T * p_self, uint8_t * p_data, size_t bit_len) {
    ASSERT(p_self != NULL);
    ASSERT(p_data != NULL);

    if(Stream_GetLeftBits(p_self) < bit_len)
        return ERROR_STREAM_TOO_SHORT;

    size_t offset = 0;
    if(bit_len % BITS_IN_BYTE != 0 && p_self->mode == BIG)
        offset += BITS_IN_BYTE - (bit_len % BITS_IN_BYTE);

    for(size_t foreign_index = offset; foreign_index < bit_len + offset;) {
        size_t    foreign_size = MIN(BITS_IN_BYTE - BIT_IN_BYTE(foreign_index), (bit_len + offset) - foreign_index);
        size_t    own_size     = MIN(BITS_IN_BYTE - Stream_TellBitInByte(p_self), Stream_GetLeftBits(p_self));
        size_t    move_size    = MIN(foreign_size, own_size);
        uint8_t * output       = p_data + BYTE_INDEX(foreign_index);
        uint8_t * input        = p_self->p_buffer + Stream_Tell(p_self);

        uint8_t data_to_write = Stream_ReadPartByte(*input, p_self->bit_index, move_size, p_self->mode);
        Stream_WritePartByte(output, data_to_write, foreign_index, move_size, p_self->mode);

        p_self->bit_index += move_size;
        foreign_index     += move_size;
    }

    return STATUS_SUCCESS;
}

static uint8_t Stream_GetMask(size_t start, size_t stop) {
    ASSERT(start <= stop);

    uint8_t lower_mask = (uint8_t) (0xFF << start);
    uint8_t upper_mask = (uint8_t) (0xFF >> (BITS_IN_BYTE - stop));

    return lower_mask & upper_mask;
}

static void Stream_WritePartByte(uint8_t *p_dst, uint8_t value, size_t index, size_t bit_count, Stream_Mode_T mode) {
    ASSERT(p_dst != NULL);

    index %= BITS_IN_BYTE;
    size_t  start_index = mode == LITTLE ? index : BITS_IN_BYTE - index - bit_count;
    size_t  end_index   = mode == LITTLE ? index + bit_count : BITS_IN_BYTE - index;
    uint8_t mask        = Stream_GetMask(start_index, end_index);

    (*p_dst) &= (uint8_t) (~mask);
    (*p_dst) |= (uint8_t) (value << BIT_IN_BYTE(start_index));
}

static uint8_t Stream_ReadPartByte(uint8_t value, size_t index, size_t bit_count, Stream_Mode_T mode) {
    index %= BITS_IN_BYTE;
    size_t  start_index = mode == LITTLE ? index : BITS_IN_BYTE - index - bit_count;
    size_t  end_index   = mode == LITTLE ? index + bit_count : BITS_IN_BYTE - index;
    uint8_t mask        = Stream_GetMask(start_index, end_index);

    return (uint8_t) ((value & mask) >> BIT_IN_BYTE(start_index));
}
