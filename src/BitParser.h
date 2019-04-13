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

#ifndef BIT_PARSER_H
#define BIT_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "Stream.h"
#include "BitParserError.h"

#define BIT_FIELD_U8(width, type, field) {.field_type = U8, .u8_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_I8(width, type, field) {.field_type = I8, .i8_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_S8(width, type, field) {.field_type = S8, .s8_f = {.offset = offsetof(type, field), .bit = (width)}}

#define BIT_FIELD_U16(width, type, field) {.field_type = U16, .u16_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_I16(width, type, field) {.field_type = I16, .i16_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_S16(width, type, field) {.field_type = S16, .s16_f = {.offset = offsetof(type, field), .bit = (width)}}

#define BIT_FIELD_U32(width, type, field) {.field_type = U32, .u32_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_I32(width, type, field) {.field_type = I32, .i32_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_S32(width, type, field) {.field_type = S32, .s32_f = {.offset = offsetof(type, field), .bit = (width)}}

#define BIT_FIELD_U64(width, type, field) {.field_type = U64LE, .u64_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_I64(width, type, field) {.field_type = I64LE, .i64_f = {.offset = offsetof(type, field), .bit = (width)}}
#define BIT_FIELD_S64(width, type, field) {.field_type = S64LE, .s64_f = {.offset = offsetof(type, field), .bit = (width)}}

#define BIT_FIELD_FLOAT(type, field)  {.field_type = FLOAT, . float_f  = {.offset = offsetof(type, field)}}
#define BIT_FIELD_DOUBLE(type, field) {.field_type = DOUBLE, .double_f = {.offset = offsetof(type, field)}}

#define BIT_FIELD_LEN(width, type, field) {.field_type = LEN, .len_f = {.offset = offsetof(type, field), .bit = (width)}}

#define BIT_FIELD_ARRAY_FIXED(_len, type, field)    {.field_type = ARRAY_FIXED,    .array_fixed_f    = {.offset = offsetof(type, field), .len = (_len)}}
#define BIT_FIELD_ARRAY_VARIABLE(type, field, _len) {.field_type = ARRAY_VARIABLE, .array_variable_f = {.offset = offsetof(type, field), .len_offset = (offsetof(type, _len))}}

#define BIT_FIELD_ALIGN()    {.field_type = ALIGN}
#define BIT_FIELD_PAD(width) {.field_type = PAD, .pad_f = {.bit = width}}

#define BIT_FIELD_U8_ENABLED
#define BIT_FIELD_I8_ENABLED
#define BIT_FIELD_S8_ENABLED
#define BIT_FIELD_U16_ENABLED
#define BIT_FIELD_I16_ENABLED
#define BIT_FIELD_S16_ENABLED
#define BIT_FIELD_U32_ENABLED
#define BIT_FIELD_I32_ENABLED
#define BIT_FIELD_S32_ENABLED
#define BIT_FIELD_U64_ENABLED
#define BIT_FIELD_I64_ENABLED
#define BIT_FIELD_S64_ENABLED
#define BIT_FIELD_FLOAT_ENABLED
#define BIT_FIELD_DOUBLE_ENABLED
#define BIT_FIELD_LEN_ENABLED
#define BIT_FIELD_ARRAY_FIXED_ENABLED
#define BIT_FIELD_ARRAY_VARIABLE_ENABLED
#define BIT_FIELD_ALIGN_ENABLED
#define BIT_FIELD_PAD_ENABLED

/**
 * Field type.
 */
typedef enum {
    U8,                 /*!< uint8_t field type. */
    I8,                 /*!< int8_t two's complement field type. */
    S8,                 /*!< int8_t sign and magnitude field type. */
    U16,                /*!< uint16_t field type. */
    I16,                /*!< int16_t two's complement field type. */
    S16,                /*!< int16_t sign and magnitude field type. */
    U32,                /*!< uint32_t field type. */
    I32,                /*!< int32_t two's complement field type. */
    S32,                /*!< int32_t sign and magnitude field type. */
    U64,                /*!< uint64_t field type. */
    I64,                /*!< int64_t two's complement field type. */
    S64,                /*!< int64_t sign and magnitude field type. */
    FLOAT,              /*!< float field type. */
    DOUBLE,             /*!< double field type. */
    LEN,                /*!< size_t field type. */
    ARRAY_FIXED,        /*!< array with fixed size field type. */
    ARRAY_VARIABLE,     /*!< array with variable size field type. */
    ALIGN,              /*!< stream align control field field type. */
    PAD,                /*!< pad control field type. */
} BitFieldType_T;

/**
 * Bit field description struct
 */
typedef struct {
    BitFieldType_T field_type;
    union {
        struct {
            size_t offset;
            size_t bit;
        } u8_f;

        struct {
            size_t offset;
            size_t bit;
        } i8_f;

        struct {
            size_t offset;
            size_t bit;
        } s8_f;

        struct {
            size_t offset;
            size_t bit;
        } u16_f;

        struct {
            size_t offset;
            size_t bit;
        } i16_f;

        struct {
            size_t offset;
            size_t bit;
        } s16_f;

        struct {
            size_t offset;
            size_t bit;
        } u32_f;

        struct {
            size_t offset;
            size_t bit;
        } i32_f;

        struct {
            size_t offset;
            size_t bit;
        } s32_f;

        struct {
            size_t offset;
            size_t bit;
        } u64_f;

        struct {
            size_t offset;
            size_t bit;
        } i64_f;

        struct {
            size_t offset;
            size_t bit;
        } s64_f;

        struct {
            size_t offset;
        } float_f;

        struct {
            size_t offset;
        } double_f;

        struct  {
            size_t offset;
            size_t bit;
        } len_f;

        struct {
            size_t offset;
            size_t len;
        } array_fixed_f;

        struct {
            size_t offset;
            size_t len_offset;
        } array_variable_f;

        struct {
            size_t bit;
        } pad_f;
    };
} BitField_T;

/**
 * Serialize struct using bit field message descriptor.
 *
 * @param p_fields      Bit field message descriptor.
 * @param no_fields     Number of fields in descriptor.
 * @param data          Structure to be serialized.
 * @param p_stream      Stream to write data.
 * @return              Status.
 */
Status_T BitParser_Serialize(const BitField_T * p_fields, size_t no_fields, void * data, Stream_T * p_stream);

/**
 * Deserialize stream into struct using bit field message descriptor.
 *
 * @param p_fields      Bit field message descriptor.
 * @param no_fields     Number of fields in descriptor.
 * @param data          Structure to write data.
 * @param p_stream      Stream to read data.
 * @return              Status.
 */
Status_T BitParser_Deserialize(const BitField_T * p_fields, size_t no_fields, void * data, Stream_T * p_stream);

/**
 * Calculate len of serialized message in bits.
 *
 * @param p_fields      Bit field message descriptor.
 * @param no_fields     Number of fields in descriptor.
 * @param data          Structure to be serialized.
 * @return              Status.
 */
size_t BitParser_GetLengthBit(const BitField_T * p_fields, size_t no_fields, void * data);

/**
 * Calculate len of serialized message in bytes.
 *
 * @param p_fields      Bit field message descriptor.
 * @param no_fields     Number of fields in descriptor.
 * @param data          Structure to be serialized.
 * @return              Status.
 */
size_t BitParser_GetLength(const BitField_T * p_fields, size_t no_fields, void * data);

#ifdef __cplusplus
}
#endif

#endif //BIT_PARSER_H
