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

#include "UParser.h"
#include "BitParserError.h"

#define GET_BYTE(data, i)    ((uint8_t) ((data) >> ((i) * 8)))
#define CREATE_BYTE(data, i) (((uint64_t) (data)) << ((i) * 8))

/**
 * Write data into a stream.
 * This function aligns stream index before writing.
 *
 * @param value         Data to write.
 * @param byte_count    Number of bytes to write.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T Serialize(uint64_t value, size_t byte_count, Stream_T * p_stream);

/**
 * Write data into a stream, using two's complement representation.
 * This function aligns stream index before writing.
 *
 * @param value         Data to write.
 * @param byte_count    Number of bytes to write.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T ISerialize(int64_t value, size_t byte_count, Stream_T * p_stream);

/**
 * Write data into a stream, using sign and magnitude representation.
 * This function aligns stream index before writing.
 *
 * @param value         Data to write.
 * @param byte_count    Number of bytes to write.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T SSerialize(int64_t value, size_t byte_count, Stream_T * p_stream);

/**
 * Write data into a stream in big endian order.
 * This function aligns stream index before writing.
 *
 * @param value         Data to write.
 * @param byte_count    Number of bytes to write.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T SerializeBE(uint64_t value, size_t byte_count, Stream_T * p_stream);

/**
 * Write data into a stream in little endian order.
 * This function aligns stream index before writing.
 *
 * @param value         Data to write.
 * @param byte_count    Number of bytes to write.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T SerializeLE(uint64_t value, size_t byte_count, Stream_T * p_stream);

/**
 * Write data into stream.
 *
 * If the stream is in LITTLE mode, writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than byte_count * 8, surplus will be filled with zeroes
 * AFTER writing actual data.
 *
 * If the stream is in BIG mode, writes bit in big endian order,
 * so if value needs to be split between bytes, the most significant bits will be written to
 * prior byte in stream. If bit_width is bigger than byte_count * 8, surplus will be filled with zeroes
 * BEFORE writing actual data.
 *
 * @param p_data     Pointer to data
 * @param byte_count Number bytes to write.
 * @param bit_width  Number of bits to actually write to stream.
 * @param p_stream   Pointer to stream
 * @return           Status.
 */
static Status_T SerializeBit(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Write data into a stream, using two's complement representation. Calls SerializeBit.
 *
 * @param p_data     Pointer to data
 * @param byte_count Number bytes to write.
 * @param bit_width  Number of bits to actually write to stream.
 * @param p_stream   Pointer to stream
 * @return           Status.
 */
static Status_T ISerializeBit(int64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Write data into a stream, using sign and magnitude representation. Calls SerializeBit.
 *
 * @param p_data     Pointer to data
 * @param byte_count Number bytes to write.
 * @param bit_width  Number of bits to actually write to stream.
 * @param p_stream   Pointer to stream
 * @return           Status.
 */
static Status_T SSerializeBit(int64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Write data into stream. Writes only bit_width bits. Writes bit in big endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than byte_count * 8, surplus will be filled with
 * zeroes AFTER writing actual data.
 *
 * @param p_data     Pointer to data
 * @param byte_count Number bytes to write.
 * @param bit_width  Number of bits to actually write to stream.
 * @param p_stream   Pointer to stream
 * @return           Status.
 */
static Status_T SerializeBitBE(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Write data into stream. Writes only bit_width bits. Writes bit in little endian order,
 * so if value needs to be split between bytes, the least significant bits will be written to
 * prior byte in stream. If bit_width is bigger than byte_count * 8, surplus will be filled with
 * zeroes BEFORE writing actual data.
 *
 * @param p_data     Pointer to data
 * @param byte_count Number bytes to write.
 * @param bit_width  Number of bits to actually write to stream.
 * @param p_stream   Pointer to stream
 * @return           Status.
 */
static Status_T SerializeBitLE(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Reads data from a stream.
 * This function aligns stream index before reading.
 *
 * @param p_data        Pointer to data.
 * @param byte_count    Number of bytes to read.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T Deserialize(uint64_t * p_output, size_t byte_count, Stream_T * p_stream);

/**
 * Reads data from a stream, using two's complement representation.
 * This function aligns stream index before reading.
 *
 * @param p_data        Pointer to data.
 * @param byte_count    Number of bytes to read.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T IDeserialize(int64_t * p_output, size_t byte_count, Stream_T * p_stream);

/**
 * Reads data from a stream, using sign and magnitude representation.
 * This function aligns stream index before reading.
 *
 * @param p_data        Pointer to data.
 * @param byte_count    Number of bytes to read.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T SDeserialize(int64_t * p_output, size_t byte_count, Stream_T * p_stream);

/**
 * Reads data from a stream in big endian order.
 * This function aligns stream index before reading.
 *
 * @param p_data        Pointer to data.
 * @param byte_count    Number of bytes to read.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T DeserializeBE(uint64_t * p_output, size_t byte_count, Stream_T * p_stream);

/**
 * Reads data from a stream in little endian order.
 * This function aligns stream index before reading.
 *
 * @param p_data        Pointer to data.
 * @param byte_count    Number of bytes to read.
 * @param p_stream      Pointer to stream.
 * @return              Status.
 */
static Status_T DeserializeLE(uint64_t * p_output, size_t byte_count, Stream_T * p_stream);

/**
 * Read data from a stream.
 *
 * If stream is in LITTLE mode, reads only bit_width bits. Reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than byte_count * 8, surplus will be skipped AFTER reading actual
 * data.
 *
 * If stream is in LITTLE mode, reads only bit_width bits. Reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than byte_count * 8, surplus will be skipped BEFORE reading actual
 * data.
 *
 * @param p_data        Pointer to data
 * @param byte_count    Number of bytes to read.
 * @param bit_width     Number of bits to actually write to stream.
 * @param p_stream      Pointer to stream
 * @return              Status.
 */
static Status_T DeserializeBit(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Reads data from a stream, using two's complement representation. Calls DeserializeBit
 *
 * @param p_data        Pointer to data
 * @param byte_count    Number of bytes to read.
 * @param bit_width     Number of bits to actually write to stream.
 * @param p_stream      Pointer to stream
 * @return              Status.
 */
static Status_T IDeserializeBit(int64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Reads data from a stream, using sign and magnitude representation. Calls DeserializeBit
 *
 * @param p_data        Pointer to data
 * @param byte_count    Number of bytes to read.
 * @param bit_width     Number of bits to actually write to stream.
 * @param p_stream      Pointer to stream
 * @return              Status.
 */
static Status_T SDeserializeBit(int64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Read data from a stream. Reads only bit_width bits. Reads bit in little endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than byte_count * 8, surplus will be skipped AFTER reading actual
 * data.
 *
 * @param p_data        Pointer to data
 * @param byte_count    Number of bytes to read.
 * @param bit_width     Number of bits to actually write to stream.
 * @param p_stream      Pointer to stream
 * @return              Status.
 */
static Status_T DeserializeBitLE(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream);

/**
 * Read data from a stream. Reads only bit_width bits. Reads bit in big endian order,
 * so if value is split between bytes, the least significant bits will be read from prior byte in
 * stream. If bit_width is bigger than byte_count * 8, surplus will be skipped BEFORE reading actual
 * data.
 *
 * @param p_data        Pointer to data
 * @param byte_count    Number of bytes to read.
 * @param bit_width     Number of bits to actually write to stream.
 * @param p_stream      Pointer to stream
 * @return              Status.
 */
static Status_T DeserializeBitBE(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream);

Status_T U8_Serialize(uint8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Serialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T I8_Serialize(int8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T S8_Serialize(int8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T U16_Serialize(uint16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Serialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T I16_Serialize(int16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T S16_Serialize(int16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T U32_Serialize(uint32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Serialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T I32_Serialize(int32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T S32_Serialize(int32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T U64_Serialize(uint64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Serialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T I64_Serialize(int64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T S64_Serialize(int64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerialize(*p_data, sizeof(*p_data), p_stream);
}

Status_T Float_Serialize(float * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U32_Serialize((uint32_t *) p_data, p_stream);
}

Status_T Double_Serialize(double * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U64_Serialize((uint64_t *) p_data, p_stream);
}

Status_T Size_Serialize(size_t * p_data, size_t byte_size, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Serialize(*p_data, byte_size, p_stream);
}

Status_T Array_Serialize(uint8_t * p_data, size_t len, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    Stream_Align(p_stream);
    return Stream_Write(p_stream, p_data, len);
}

Status_T U8_SerializeBit(uint8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T I8_SerializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T S8_SerializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T U16_SerializeBit(uint16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T I16_SerializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T S16_SerializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T U32_SerializeBit(uint32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T I32_SerializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T S32_SerializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T U64_SerializeBit(uint64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T I64_SerializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return ISerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T S64_SerializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SSerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T Float_SerializeBit(float * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U32_SerializeBit((uint32_t *) p_data, sizeof(*p_data) * BITS_IN_BYTE, p_stream);
}

Status_T Double_SerializeBit(double * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U64_SerializeBit((uint64_t *) p_data, sizeof(*p_data) * BITS_IN_BYTE, p_stream);
}

Status_T Size_SerializeBit(size_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SerializeBit(*p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T Array_SerializeBit(uint8_t * p_data, size_t data_len, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    if(Stream_GetLeftBits(p_stream) < data_len * BITS_IN_BYTE)
        return ERROR_STREAM_TOO_SHORT;

    for(size_t i = 0; i < data_len; i++) {
        Status_T result = U8_SerializeBit(p_data + i, BITS_IN_BYTE, p_stream);
        if(result != STATUS_SUCCESS)
            return result;
    }

    return STATUS_SUCCESS;
}

Status_T U8_Deserialize(uint8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Stream_Read(p_stream, p_data, sizeof(uint8_t));
}

Status_T I8_Deserialize(int8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Stream_Read(p_stream, (uint8_t *) p_data, sizeof(uint8_t));
}

Status_T S8_Deserialize(int8_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t data;
    Status_T result = SDeserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int8_t) data;
    return STATUS_SUCCESS;
}

Status_T U16_Deserialize(uint16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t data;
    Status_T result = Deserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (uint16_t) data;
    return STATUS_SUCCESS;
}

Status_T I16_Deserialize(int16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t data;
    Status_T result = IDeserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int16_t) data;
    return STATUS_SUCCESS;
}

Status_T S16_Deserialize(int16_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t data;
    Status_T result = SDeserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int16_t) data;
    return STATUS_SUCCESS;
}

Status_T U32_Deserialize(uint32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t data;
    Status_T result = Deserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (uint32_t) data;
    return STATUS_SUCCESS;
}

Status_T I32_Deserialize(int32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t data;
    Status_T result = IDeserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int32_t) data;
    return STATUS_SUCCESS;
}

Status_T S32_Deserialize(int32_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t data;
    Status_T result = SDeserialize(&data, sizeof(*p_data), p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int32_t) data;
    return STATUS_SUCCESS;
}

Status_T U64_Deserialize(uint64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Deserialize(p_data, sizeof(*p_data), p_stream);
}

Status_T I64_Deserialize(int64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return IDeserialize(p_data, sizeof(*p_data), p_stream);
}

Status_T S64_Deserialize(int64_t * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SDeserialize(p_data, sizeof(*p_data), p_stream);
}

Status_T Float_Deserialize(float * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U32_Deserialize((uint32_t *) p_data, p_stream);
}

Status_T Double_Deserialize(double * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U64_Deserialize((uint64_t *) p_data, p_stream);
}

Status_T Size_Deserialize(size_t * p_data, size_t byte_size, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t value;
    Status_T result = Deserialize(&value, byte_size, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (size_t) value;

    return result;
}

Status_T Array_Deserialize(uint8_t * p_data, size_t len, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return Stream_Read(p_stream, p_data, len);
}

Status_T U8_DeserializeBit(uint8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t value;
    Status_T result = DeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (uint8_t) value;

    return STATUS_SUCCESS;
}

Status_T I8_DeserializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = IDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int8_t) value;

    return STATUS_SUCCESS;
}

Status_T S8_DeserializeBit(int8_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = SDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int8_t) value;

    return STATUS_SUCCESS;
}

Status_T U16_DeserializeBit(uint16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t value;
    Status_T result = DeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (uint16_t) value;

    return STATUS_SUCCESS;
}

Status_T I16_DeserializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = IDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int16_t) value;

    return STATUS_SUCCESS;
}

Status_T S16_DeserializeBit(int16_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = SDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int16_t) value;

    return STATUS_SUCCESS;
}

Status_T U32_DeserializeBit(uint32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t value;
    Status_T result = DeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (uint32_t) value;

    return STATUS_SUCCESS;
}

Status_T I32_DeserializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = IDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int32_t) value;

    return STATUS_SUCCESS;
}

Status_T S32_DeserializeBit(int32_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    int64_t value;
    Status_T result = SDeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (int32_t) value;

    return STATUS_SUCCESS;
}

Status_T U64_DeserializeBit(uint64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return DeserializeBit(p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T I64_DeserializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return IDeserializeBit(p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T S64_DeserializeBit(int64_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return SDeserializeBit(p_data, sizeof(*p_data), bit_width, p_stream);
}

Status_T Float_DeserializeBit(float * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U32_DeserializeBit((uint32_t *) p_data, sizeof(*p_data) * BITS_IN_BYTE, p_stream);
}

Status_T Double_DeserializeBit(double * p_data, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    return U64_DeserializeBit((uint64_t *) p_data, sizeof(*p_data) * BITS_IN_BYTE, p_stream);
}

Status_T Size_DeserializeBit(size_t * p_data, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t value;
    Status_T result = DeserializeBit(&value, sizeof(*p_data), bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = (size_t) value;

    return STATUS_SUCCESS;
}

Status_T Array_DeserializeBit(uint8_t * p_data, size_t data_len, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    if(Stream_GetLeftBits(p_stream) < data_len * BITS_IN_BYTE)
        return ERROR_STREAM_TOO_SHORT;

    Status_T result = STATUS_SUCCESS;
    for(size_t i = 0; i < data_len; i++) {
        result &= U8_DeserializeBit(p_data + i, BITS_IN_BYTE, p_stream);
    }

    return result;
}

/*======================================================================================*/
/*                   ####### LOCAL FUNCTIONS DEFINITIONS #######                        */
/*======================================================================================*/

static Status_T Serialize(uint64_t value, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    if(Stream_GetMode(p_stream) == LITTLE)
        return SerializeLE(value, byte_count, p_stream);
    else
        return SerializeBE(value, byte_count, p_stream);
}

static Status_T ISerialize(int64_t value, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint64_t x;
    memcpy(&x, &value, sizeof(value));

    return Serialize(x, byte_count, p_stream);
}

static Status_T SSerialize(int64_t value, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint64_t x;

    if(value < 0) {
        x  = (uint64_t) (value * -1);
        x |= (uint64_t) (1 << ((BITS_IN_BYTE * byte_count) - 1));
    }
    else {
        x = (uint64_t) value;
    }

    return Serialize(x, byte_count, p_stream);
}

static Status_T SerializeBE(uint64_t value, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    for(size_t i = 0; i < byte_count; i++)
        data[i] = GET_BYTE(value, byte_count - i - 1);

    return Stream_Write(p_stream, data, sizeof(data));
}

static Status_T SerializeLE(uint64_t value, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    for(size_t i = 0; i < byte_count; i++)
        data[i] = GET_BYTE(value, i);

    return Stream_Write(p_stream, data, sizeof(data));
}

static Status_T SerializeBit(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    if(Stream_GetMode(p_stream) == LITTLE)
        return SerializeBitLE(value, byte_count, bit_width, p_stream);
    else
        return SerializeBitBE(value, byte_count, bit_width, p_stream);
}

static Status_T ISerializeBit(int64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint64_t x;
    memcpy(&x, &value, sizeof(value));

    return SerializeBit(x, byte_count, bit_width, p_stream);
}

static Status_T SSerializeBit(int64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint64_t x;

    if(value < 0) {
        x  = (uint64_t) (value * -1);
        x |= (uint64_t) (1 << (bit_width - 1));
    }
    else {
        x = (uint64_t) value;
    }

    return SerializeBit(x, byte_count, bit_width, p_stream);
}

static Status_T SerializeBitLE(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    for(size_t i = 0; i < byte_count; i++)
        data[i] = GET_BYTE(value, i);

    if(Stream_GetLeftBits(p_stream) < bit_width)
        return ERROR_STREAM_TOO_SHORT;

    size_t bit_to_be_written = MIN(bit_width, BITS_IN_BYTE * sizeof(data));
    Status_T result = Stream_WriteBit(p_stream, data, bit_to_be_written);
    if(result != STATUS_SUCCESS)
        return result;

    if(bit_width > BITS_IN_BYTE * sizeof(data))
        result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + bit_width - BITS_IN_BYTE * sizeof(data));

    return result;
}

static Status_T SerializeBitBE(uint64_t value, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    for(size_t i = 0; i < byte_count; i++)
        data[i] = GET_BYTE(value, byte_count - i - 1);

    if(Stream_GetLeftBits(p_stream) < bit_width)
        return ERROR_STREAM_TOO_SHORT;

    Status_T result = STATUS_SUCCESS;
    if(bit_width > BITS_IN_BYTE * sizeof(data))
        result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + bit_width - BITS_IN_BYTE * sizeof(data));
    if(result != STATUS_SUCCESS)
        return result;

    size_t offset = 0;
    if(bit_width <= (byte_count - 1) * BITS_IN_BYTE)
        offset = byte_count - (bit_width / BITS_IN_BYTE) - (bit_width % BITS_IN_BYTE ? 1 : 0);

    size_t bit_to_be_written = MIN(bit_width, BITS_IN_BYTE * sizeof(data));
    return Stream_WriteBit(p_stream, data + offset, bit_to_be_written);
}

static Status_T Deserialize(uint64_t * p_output, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_output != NULL);
    ASSERT(p_stream != NULL);

    if(Stream_GetMode(p_stream) == LITTLE)
        return DeserializeLE(p_output, byte_count, p_stream);
    else
        return DeserializeBE(p_output, byte_count, p_stream);
}

static Status_T IDeserialize(int64_t * p_output, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_output != NULL);
    ASSERT(p_stream != NULL);

    uint64_t x;

    Status_T result = Deserialize(&x, byte_count, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    memcpy(p_output, &x, byte_count);

    return STATUS_SUCCESS;
}

static Status_T SDeserialize(int64_t * p_output, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_output != NULL);
    ASSERT(p_stream != NULL);

    uint64_t x;

    Status_T result = Deserialize(&x, byte_count, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    uint64_t mask = (uint64_t) 1 << ((byte_count * BITS_IN_BYTE) - 1);

    (*p_output) = (int64_t) (x & (~mask));
    if(x & mask)
        (*p_output) *= -1;

    return STATUS_SUCCESS;
}

static Status_T DeserializeBE(uint64_t * p_output, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_output != NULL);
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    Status_T result = Stream_Read(p_stream, data, byte_count);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_output) = 0;
    for(size_t i = 0; i < byte_count; i++)
        (*p_output) |= CREATE_BYTE(data[byte_count - i - 1], i);

    return STATUS_SUCCESS;
}

static Status_T DeserializeLE(uint64_t * p_output, size_t byte_count, Stream_T * p_stream) {
    ASSERT(p_output != NULL);
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];

    Status_T result = Stream_Read(p_stream, data, byte_count);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_output) = 0;
    for(size_t i = 0; i < byte_count; i++)
        (*p_output) |= CREATE_BYTE(data[i], i);

    return STATUS_SUCCESS;
}

static Status_T DeserializeBit(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    if(Stream_GetMode(p_stream) == LITTLE)
        return DeserializeBitLE(p_data, byte_count, bit_width, p_stream);
    else
        return DeserializeBitBE(p_data, byte_count, bit_width, p_stream);
}

static Status_T IDeserializeBit(int64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t x;

    Status_T result = DeserializeBit(&x, byte_count, bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    uint64_t mask = (uint64_t) 1 << (bit_width - 1);

    if(x & mask)
        x |= (uint64_t) 0xFF << bit_width;

    memcpy(p_data, &x, byte_count);

    return STATUS_SUCCESS;
}

static Status_T SDeserializeBit(int64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint64_t x;

    Status_T result = DeserializeBit(&x, byte_count, bit_width, p_stream);
    if(result != STATUS_SUCCESS)
        return result;

    uint64_t mask = (uint64_t) 1 << (bit_width - 1);

    (*p_data) = (int64_t) (x & (~mask));
    if(x & mask)
        (*p_data) *= -1;

    return STATUS_SUCCESS;
}

static Status_T DeserializeBitLE(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];
    memset(data, 0, byte_count);

    if(Stream_GetLeftBits(p_stream) < bit_width)
        return ERROR_STREAM_TOO_SHORT;

    size_t bit_to_be_read = MIN(bit_width, BITS_IN_BYTE * byte_count);
    Status_T result = Stream_ReadBit(p_stream, data, bit_to_be_read);
    if(result != STATUS_SUCCESS)
        return result;

    if(bit_width > BITS_IN_BYTE * byte_count)
        result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + bit_width - BITS_IN_BYTE * byte_count);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data)  = 0;
    for(size_t i = 0; i < byte_count; i++)
        (*p_data) |= CREATE_BYTE(data[i], i);

    return STATUS_SUCCESS;
}

static Status_T DeserializeBitBE(uint64_t * p_data, size_t byte_count, size_t bit_width, Stream_T * p_stream) {
    ASSERT(p_data != NULL);
    ASSERT(p_stream != NULL);

    uint8_t data[byte_count];
    memset(data, 0, byte_count);

    if(Stream_GetLeftBits(p_stream) < bit_width)
        return ERROR_STREAM_TOO_SHORT;

    Status_T result = STATUS_SUCCESS;
    if(bit_width > BITS_IN_BYTE * byte_count)
        result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + bit_width - BITS_IN_BYTE * byte_count);
    if(result != STATUS_SUCCESS)
        return result;

    size_t bit_to_be_read = MIN(bit_width, BITS_IN_BYTE * byte_count);
    result = Stream_ReadBit(p_stream, data, bit_to_be_read);
    if(result != STATUS_SUCCESS)
        return result;

    (*p_data) = 0;

    size_t offset = bit_width < byte_count * BITS_IN_BYTE
                  ? (byte_count * BITS_IN_BYTE - bit_width) / BITS_IN_BYTE
                  : 0;

    for(size_t i = 0; i < byte_count - offset; i++)
        (*p_data) |= CREATE_BYTE(data[byte_count - i - 1 - offset], i);

    return STATUS_SUCCESS;
}
