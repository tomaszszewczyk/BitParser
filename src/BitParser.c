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

#include "BitParser.h"
#include "UParser.h"
#include "BitParserError.h"

Status_T BitParser_Serialize(const BitField_T * p_fields, size_t no_fields, void * data, Stream_T * p_stream) {
    ASSERT(p_fields != NULL);
    ASSERT(data != NULL);
    ASSERT(p_stream != NULL);

    Status_T result = STATUS_SUCCESS;

    for(size_t i = 0; i < no_fields; i++) {
        switch(p_fields[i].field_type) {
            #ifdef BIT_FIELD_U8_ENABLED
            case U8:
                result = U8_SerializeBit(data + p_fields[i].u8_f.offset, p_fields[i].u8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I8_ENABLED
            case I8:
                result = I8_SerializeBit(data + p_fields[i].i8_f.offset, p_fields[i].i8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S8_ENABLED
            case S8:
                result = S8_SerializeBit(data + p_fields[i].s8_f.offset, p_fields[i].s8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U16_ENABLED
            case U16:
                result = U16_SerializeBit(data + p_fields[i].u16_f.offset, p_fields[i].u16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I16_ENABLED
            case I16:
                result = I16_SerializeBit(data + p_fields[i].i16_f.offset, p_fields[i].i16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S16_ENABLED
            case S16:
                result = S16_SerializeBit(data + p_fields[i].s16_f.offset, p_fields[i].s16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U32_ENABLED
            case U32:
                result = U32_SerializeBit(data + p_fields[i].u32_f.offset, p_fields[i].u32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I32_ENABLED
            case I32:
                result = I32_SerializeBit(data + p_fields[i].i32_f.offset, p_fields[i].i32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S32_ENABLED
            case S32:
                result = S32_SerializeBit(data + p_fields[i].s32_f.offset, p_fields[i].s32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U64_ENABLED
            case U64:
                result = U64_SerializeBit(data + p_fields[i].u64_f.offset, p_fields[i].u64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I64_ENABLED
            case I64:
                result = I64_SerializeBit(data + p_fields[i].i64_f.offset, p_fields[i].i64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S64_ENABLED
            case S64:
                result = S64_SerializeBit(data + p_fields[i].s64_f.offset, p_fields[i].s64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_FLOAT_ENABLED
            case FLOAT:
                result = Float_SerializeBit(data + p_fields[i].float_f.offset, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_DOUBLE_ENABLED
            case DOUBLE:
                result = Double_SerializeBit(data + p_fields[i].double_f.offset, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_LEN_ENABLED
            case LEN:
                result = Size_SerializeBit(data + p_fields[i].len_f.offset, p_fields[i].len_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_FIXED_ENABLED
            case ARRAY_FIXED:
                result = Array_SerializeBit(*((uint8_t **) (data + p_fields[i].array_fixed_f.offset)),
                                             p_fields[i].array_fixed_f.len,
                                             p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_VARIABLE_ENABLED
            case ARRAY_VARIABLE:
                result = Array_SerializeBit(*((uint8_t **) (data + p_fields[i].array_variable_f.offset)),
                                             *((size_t *) (data + p_fields[i].array_variable_f.len_offset)),
                                             p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ALIGN_ENABLED
            case ALIGN:
                Stream_Align(p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_PAD_ENABLED
            case PAD:
                result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + p_fields[i].pad_f.bit);
                break;
            #endif

            default:
                ASSERT(false);
        }

        if(result != STATUS_SUCCESS)
            break;
    }

    return result;
}

Status_T BitParser_Deserialize(const BitField_T * p_fields, size_t no_fields, void * data, Stream_T * p_stream) {
    ASSERT(p_fields != NULL);
    ASSERT(data != NULL);
    ASSERT(p_stream != NULL);

    Status_T result = STATUS_SUCCESS;

    for(size_t i = 0; i < no_fields; i++) {
        switch(p_fields[i].field_type) {
            #ifdef BIT_FIELD_U8_ENABLED
            case U8:
                result = U8_DeserializeBit(data + p_fields[i].u8_f.offset, p_fields[i].u8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I8_ENABLED
            case I8:
                result = I8_DeserializeBit(data + p_fields[i].i8_f.offset, p_fields[i].i8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S8_ENABLED
            case S8:
                result = S8_DeserializeBit(data + p_fields[i].s8_f.offset, p_fields[i].s8_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U16_ENABLED
            case U16:
                result = U16_DeserializeBit(data + p_fields[i].u16_f.offset, p_fields[i].u16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I16_ENABLED
            case I16:
                result = I16_DeserializeBit(data + p_fields[i].i16_f.offset, p_fields[i].i16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S16_ENABLED
            case S16:
                result = S16_DeserializeBit(data + p_fields[i].s16_f.offset, p_fields[i].s16_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U32_ENABLED
            case U32:
                result = U32_DeserializeBit(data + p_fields[i].u32_f.offset, p_fields[i].u32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I32_ENABLED
            case I32:
                result = I32_DeserializeBit(data + p_fields[i].i32_f.offset, p_fields[i].i32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S32_ENABLED
            case S32:
                result = S32_DeserializeBit(data + p_fields[i].s32_f.offset, p_fields[i].s32_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_U64_ENABLED
            case U64:
                result = U64_DeserializeBit(data + p_fields[i].u64_f.offset, p_fields[i].u64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_I64_ENABLED
            case I64:
                result = I64_DeserializeBit(data + p_fields[i].i64_f.offset, p_fields[i].i64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_S64_ENABLED
            case S64:
                result = S64_DeserializeBit(data + p_fields[i].s64_f.offset, p_fields[i].s64_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_FLOAT_ENABLED
            case FLOAT:
                result = Float_DeserializeBit(data + p_fields[i].float_f.offset, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_DOUBLE_ENABLED
            case DOUBLE:
                result = Double_DeserializeBit(data + p_fields[i].double_f.offset, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_LEN_ENABLED
            case LEN:
                result = Size_DeserializeBit(data + p_fields[i].len_f.offset, p_fields[i].len_f.bit, p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_FIXED_ENABLED
            case ARRAY_FIXED:
                result = Array_DeserializeBit(*((uint8_t **) (data + p_fields[i].array_fixed_f.offset)),
                                               p_fields[i].array_fixed_f.len,
                                               p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_VARIABLE_ENABLED
            case ARRAY_VARIABLE:
                result = Array_DeserializeBit(*((uint8_t **) (data + p_fields[i].array_variable_f.offset)),
                                               *((size_t *) (data + p_fields[i].array_variable_f.len_offset)),
                                               p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_ALIGN_ENABLED
            case ALIGN:
                Stream_Align(p_stream);
                break;
            #endif

            #ifdef BIT_FIELD_PAD_ENABLED
            case PAD:
                result = Stream_SeekBit(p_stream, Stream_TellBit(p_stream) + p_fields[i].pad_f.bit);
                break;
            #endif

            default:
                ASSERT(false);
        }

        if(result != STATUS_SUCCESS)
            break;
    }

    return result;
}

size_t BitParser_GetLengthBit(const BitField_T * p_fields, size_t no_fields, void * data)  {
    ASSERT(p_fields != NULL);

    size_t result = 0;

    for(size_t i = 0; i < no_fields; i++) {
        switch(p_fields[i].field_type) {
            #ifdef BIT_FIELD_U8_ENABLED
            case U8:
                result += p_fields[i].u8_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_I8_ENABLED
            case I8:
                result += p_fields[i].i8_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_S8_ENABLED
            case S8:
                result += p_fields[i].s8_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_U16_ENABLED
            case U16:
                result += p_fields[i].u16_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_I16_ENABLED
            case I16:
                result += p_fields[i].i16_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_S16_ENABLED
            case S16:
                result += p_fields[i].s16_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_U32_ENABLED
            case U32:
                result += p_fields[i].u32_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_I32_ENABLED
            case I32:
                result += p_fields[i].i32_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_S32_ENABLED
            case S32:
                result += p_fields[i].s32_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_U64_ENABLED
            case U64:
                result += p_fields[i].u64_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_I64_ENABLED
            case I64:
                result += p_fields[i].i64_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_S64_ENABLED
            case S64:
                result += p_fields[i].s64_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_FLOAT_ENABLED
            case FLOAT:
                result += sizeof(float);
                break;
            #endif

            #ifdef BIT_FIELD_DOUBLE_ENABLED
            case DOUBLE:
                result += sizeof(double);
                break;
            #endif

            #ifdef BIT_FIELD_LEN_ENABLED
            case LEN:
                result += p_fields[i].len_f.bit;
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_FIXED_ENABLED
            case ARRAY_FIXED:
                result += p_fields[i].array_fixed_f.len;
                break;
            #endif

            #ifdef BIT_FIELD_ARRAY_VARIABLE_ENABLED
            case ARRAY_VARIABLE:
                ASSERT(data != NULL);
                result += *((size_t *) (data + p_fields[i].array_variable_f.len_offset)) * BITS_IN_BYTE;
                break;
            #endif

            #ifdef BIT_FIELD_ALIGN_ENABLED
            case ALIGN:
                result += BITS_IN_BYTE - (result % BITS_IN_BYTE);
                break;
            #endif

            #ifdef BIT_FIELD_PAD_ENABLED
            case PAD:
                result += p_fields[i].pad_f.bit;
                break;
            #endif

            default:
                ASSERT(false);
        }
    }

    return result;
}

size_t BitParser_GetLength(const BitField_T * p_fields, size_t no_fields, void * data) {
    ASSERT(p_fields != NULL);

    size_t bit = BitParser_GetLengthBit(p_fields, no_fields, data);
    return bit / BITS_IN_BYTE + (bit % BITS_IN_BYTE != 0 ? 1 : 0);
}
