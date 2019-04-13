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

#ifndef U_PARSER_H
#define U_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "Stream.h"
#include "BitParserError.h"

/**
 * Write uint8 data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U8_Serialize(uint8_t * p_data, Stream_T * p_stream);

/**
 * Write int8 data into a stream, using two's complement representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I8_Serialize(int8_t * p_data, Stream_T * p_stream);

/**
 * Write int8 data into a stream, using sign and magnitude representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S8_Serialize(int8_t * p_data, Stream_T * p_stream);

/**
 * Write uint16 data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U16_Serialize(uint16_t * p_data, Stream_T * p_stream);

/**
 * Write int16 data into a stream, using two's complement representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I16_Serialize(int16_t * p_data, Stream_T * p_stream);

/**
 * Write int16 data into a stream, using sign and magnitude representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S16_Serialize(int16_t * p_data, Stream_T * p_stream);

/**
 * Write uint32 data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U32_Serialize(uint32_t * p_data, Stream_T * p_stream);

/**
 * Write int32 data into a stream, using two's complement representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I32_Serialize(int32_t * p_data, Stream_T * p_stream);

/**
 * Write int32 data into a stream, using sign and magnitude representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S32_Serialize(int32_t * p_data, Stream_T * p_stream);

/**
 * Write uint64 data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U64_Serialize(uint64_t * p_data, Stream_T * p_stream);

/**
 * Write int64 data into a stream, using two's complement representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I64_Serialize(int64_t * p_data, Stream_T * p_stream);

/**
 * Write int64 data into a stream, using sign and magnitude representation.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S64_Serialize(int64_t * p_data, Stream_T * p_stream);

/**
 * Write float data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Float_Serialize(float * p_data, Stream_T * p_stream);

/**
 * Write double data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Double_Serialize(double * p_data, Stream_T * p_stream);

/**
 * Write size_t data into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Size_Serialize(size_t * p_data, size_t byte_size, Stream_T * p_stream);

/**
 * Write array into a stream.
 * This function aligns stream index before writing.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Array_Serialize(uint8_t * p_data, size_t len, Stream_T * p_stream);

/**
 * Write uint8 data into stream. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U8_SerializeBit(uint8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int8 data into stream, using two's complement representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I8_SerializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int8 data into stream, using sign and magnitude representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 8, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S8_SerializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write uint16 data into stream. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U16_SerializeBit(uint16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int16 data into stream, using two's complement representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I16_SerializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int16 data into stream, using sign and magnitude representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 16, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S16_SerializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write uint32 data into stream. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U32_SerializeBit(uint32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int32 data into stream, using two's complement representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I32_SerializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int32 data into stream, using sign and magnitude representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 32, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S32_SerializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write uint64 data into stream. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U64_SerializeBit(uint64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int64 data into stream, using two's complement representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I64_SerializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write int64 data into stream, using sign and magnitude representation. Writes only bit_width bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than 64, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S64_SerializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write float data into stream.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Float_SerializeBit(float * p_data, Stream_T * p_stream);

/**
 * Write double data into stream.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream.
 *
 * @param p_data    Pointer to data
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Double_SerializeBit(double * p_data, Stream_T * p_stream);

/**
 * Write size_t data into stream.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than size_t size, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than size_t size, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Size_SerializeBit(size_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Write data into stream. Writes only data_len * 8 bits.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream.
 *
 * @param p_data    Pointer to data
 * @param data_len  Number of bytes to write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Array_SerializeBit(uint8_t * p_data, size_t data_len, Stream_T * p_stream);

/**
 * Reads uint8 data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U8_Deserialize(uint8_t * p_data, Stream_T * p_stream);

/**
 * Reads int8 data from a stream, using two's complement representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I8_Deserialize(int8_t * p_data, Stream_T * p_stream);

/**
 * Reads int8 data from a stream, using sign and magnitude representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S8_Deserialize(int8_t * p_data, Stream_T * p_stream);

/**
 * Reads uint16 data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U16_Deserialize(uint16_t * p_data, Stream_T * p_stream);

/**
 * Reads int16 data from a stream, using two's complement representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I16_Deserialize(int16_t * p_data, Stream_T * p_stream);

/**
 * Reads int16 data from a stream, using sign and magnitude representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S16_Deserialize(int16_t * p_data, Stream_T * p_stream);

/**
 * Reads uint32 data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U32_Deserialize(uint32_t * p_data, Stream_T * p_stream);

/**
 * Reads int32 data from a stream, using two's complement representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I32_Deserialize(int32_t * p_data, Stream_T * p_stream);

/**
 * Reads int32 data from a stream, using sign and magnitude representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S32_Deserialize(int32_t * p_data, Stream_T * p_stream);

/**
 * Reads uint32 data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T U64_Deserialize(uint64_t * p_data, Stream_T * p_stream);

/**
 * Reads int64 data from a stream, using two's complement representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T I64_Deserialize(int64_t * p_data, Stream_T * p_stream);

/**
 * Reads int64 data from a stream, using sign and magnitude representation.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T S64_Deserialize(int64_t * p_data, Stream_T * p_stream);

/**
 * Reads float data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Float_Deserialize(float * p_data, Stream_T * p_stream);

/**
 * Reads double data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Double_Deserialize(double * p_data, Stream_T * p_stream);

/**
 * Reads size_t data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Size_Deserialize(size_t * p_data, size_t byte_size, Stream_T * p_stream);

/**
 * Reads data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data    Pointer to data.
 * @param p_stream  Pointer to stream.
 * @return          Status.
 */
Status_T Array_Deserialize(uint8_t * p_data, size_t len, Stream_T * p_stream);

/**
 * Read uint8 data from a stream. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U8_DeserializeBit(uint8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int8 data from a stream, using two's complement representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I8_DeserializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int8 data from a stream, using sign and magnitude representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 8, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S8_DeserializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read uint16 data from a stream. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U16_DeserializeBit(uint16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int16 data from a stream, using two's complement representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I16_DeserializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int16 data from a stream, using sign and magnitude representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 16, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S16_DeserializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read uint32 data from a stream. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U32_DeserializeBit(uint32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int32 data from a stream, using two's complement representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I32_DeserializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int32 data from a stream, using sign and magnitude representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 32, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S32_DeserializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read uint64 data from a stream. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T U64_DeserializeBit(uint64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int64 data from a stream, using two's complement representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T I64_DeserializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read int64 data from a stream, using sign and magnitude representation. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than 64, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T S64_DeserializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read float data from a stream. Does not allow for bit width manipulation.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * @param p_data    Pointer to data
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Float_DeserializeBit(float * p_data, Stream_T * p_stream);

/**
 * Read double data from a stream. Does not allow for bit width manipulation.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * @param p_data    Pointer to data
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Double_DeserializeBit(double * p_data, Stream_T * p_stream);

/**
 * Read size_t data from a stream. Reads only bit_width bits.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than size_t, surplus will be skipped AFTER reading actual data.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than size_t, surplus will be skipped BEFORE reading actual data.
 *
 * @param p_data    Pointer to data
 * @param bit_width Number of bits to actually write to stream.
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Size_DeserializeBit(size_t * p_data, size_t bit_width, Stream_T * p_stream);

/**
 * Read data from a stream. Does not allow for bit width manipulation.
 *
 * If stream is in LITTLE mode, reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * If stream is in BIG mode, reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream.
 *
 * @param p_data    Pointer to data
 * @param p_stream  Pointer to stream
 * @return          Status.
 */
Status_T Array_DeserializeBit(uint8_t * p_data, size_t data_len, Stream_T * p_stream);

#ifdef __cplusplus
}
#endif

#endif //U_PARSER_H
