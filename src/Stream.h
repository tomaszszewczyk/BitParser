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

#ifndef STREAM_H
#define STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "BitParserError.h"

#define MIN(x, y)    ((x) < (y) ? (x) : (y))
#define BITS_IN_BYTE 8u
#define ARRAY_LEN(x) ((sizeof(x)) / sizeof(*x))

/**
 * Stream mode type.
 */
typedef enum {
    BIG,    /*!< Stream in big endian mode. Most significant bits are considered first. */
    LITTLE  /*!< Stream in little endian mode. Least significant bits are considered first. */
} Stream_Mode_T;

/**
 *  Stream is a data structure describing binary read and write stream.
 *
 *  Stream can be used for convenient serialization and deserialization. It handles buffer length and
 *  current index automatically in order to avoid common mistakes and reduce code written. What is more
 *  is allows to parse data in both byte mode and bit mode. In bit mode stream can parse field with
 *  given with. I. e. it can parse two 12bit fields fitted into three bytes.
 */
typedef struct {
    uint8_t *     p_buffer;     /*!< Pointer to external buffer with actual data. */
    size_t        bit_len;      /*!< Buffer length in bits. */
    size_t        bit_index;    /*!< Current bit index. */
    Stream_Mode_T mode;         /*!< Stream mode */
} Stream_T;

/**
 * Initialize stream.
 *
 * @param p_self    Pointer to allocated stream object.
 * @param p_buffer  Pointer to data buffer.
 * @param len       Length of data buffer in bytes.
 * @param mode      Stream mode
 */
void Stream_Init(Stream_T * p_self, uint8_t * p_buffer, size_t len, Stream_Mode_T mode);

/**
 * Get current stream mode.
 *
 * @param p_self    Pointer to stream object.
 * @return          Current stream mode.
 */
Stream_Mode_T Stream_GetMode(Stream_T * p_self);

/**
 * Set new stream mode. If stream is not aligned mode cannot be changed.
 *
 * @param p_self    Pointer to stream object.
 * @param mode      New stream mode.
 * @return          Status.
 */
Status_T Stream_SetMode(Stream_T * p_self, Stream_Mode_T mode);

/**
 * Get size of stream's buffer.
 *
 * @param p_self    Pointer to stream object.
 * @return          Stream's buffer size in bytes.
 */
size_t Stream_GetSize(Stream_T * p_self);

/**
 * Get size of stream's buffer in bits.
 *
 * @param p_self    Pointer to stream object.
 * @return          Stream's buffer size in bits.
 */
size_t Stream_GetSizeBits(Stream_T * p_self);

/**
 * Get number of bytes left in stream.
 *
 * @param p_self    Pointer to stream object.
 * @return          Bytes left in stream.
 */
size_t Stream_GetLeft(Stream_T * p_self);

/**
 * Get number of bits left in stream.
 *
 * @param p_self    Pointer to stream object.
 * @return          Number of bits left in stream.
 */
size_t Stream_GetLeftBits(Stream_T * p_self);

/**
 * Get number of bits left in a current byte.
 *
 * @param p_self    Pointer to stream object.
 * @return          Number of bits left in a current byte.
 */
size_t Stream_GetLeftBitsInByte(Stream_T * p_self);

/**
 * Move stream's internal index to a given byte.
 * Using this function aligns index to a full byte.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self    Pointer to stream object.
 * @param index     Where to move internal index in bytes.
 * @return          Status.
 */
Status_T Stream_Seek(Stream_T * p_self, size_t index);

/**
 * Move stream's internal index to a given bit.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self        Pointer to stream object.
 * @param bit_index     Where to move internal index in bits.
 * @return              Status.
 */
Status_T Stream_SeekBit(Stream_T * p_self, size_t bit_index);

/**
 * Move stream's internal index to a given bit in a current byte.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self        Pointer to stream object.
 * @param bit_index     Where to move internal index in bits.
 * @return              Status.
 */
Status_T Stream_SeekBitInByte(Stream_T * p_self, size_t bit_index);

/**
 * Tell stream's internal index position in bytes.
 * If index is not aligned to bytes result will be rounded down.
 *
 * @param p_self    Pointer to stream object.
 * @return          Index in bytes.
 */
size_t Stream_Tell(Stream_T * p_self);

/**
 * Tell stream's internal index position in bits.
 *
 * @param p_self    Pointer to stream object.
 * @return          Index in bits.
 */
size_t Stream_TellBit(Stream_T * p_self);

/**
 * Tell stream's internal index position in bits in a current byte.
 *
 * @param p_self    Pointer to stream object.
 * @return          Index in bits.
 */
size_t Stream_TellBitInByte(Stream_T * p_self);

/**
 * Skip all bits in a stream since the beginning of the next byte.
 *
 * @param p_self    Pointer to stream object.
 */
void Stream_Align(Stream_T * p_self);

/**
 * Writes data to a stream.
 * This function automatically aligns index before writing.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self    Pointer to stream object.
 * @param p_data    Pointer to buffer with data to write.
 * @param len       Length of data to write in bytes.
 * @return          Status.
 */
Status_T Stream_Write(Stream_T * p_self, uint8_t * p_data, size_t len);

/**
 * Writes data to a stream.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self    Pointer to stream object.
 * @param p_data    Pointer to buffer with data to write.
 * @param bit_len   Length of data to write in bits.
 * @return          Status.
 */
Status_T Stream_WriteBit(Stream_T * p_self, uint8_t * p_data, size_t bit_len);

/**
 * Read data from a stream.
 * This function automatically aligns index before reading.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self    Pointer to stream object.
 * @param p_data    Pointer to output buffer.
 * @param len       Number of bytes to read.
 * @return          Status.
 */
Status_T Stream_Read(Stream_T * p_self, uint8_t * p_data, size_t len);

/**
 * Reads data from a stream.
 * This function will gracefully handle index exceeding buffer len.
 *
 * @param p_self    Pointer to stream object.
 * @param p_data    Pointer to output buffer.
 * @param bit_len   Length of data to read in bits.
 * @return          Status.
 */
Status_T Stream_ReadBit(Stream_T * p_self, uint8_t * p_data, size_t bit_len);

#ifdef __cplusplus
}
#endif

#endif //STREAM_H
