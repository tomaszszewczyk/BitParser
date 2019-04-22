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

#ifndef BITPARSER_MODBUS_H
#define BITPARSER_MODBUS_H

#include "BitParser.h"

typedef struct {
    uint16_t starting_address;
    uint16_t no_points;
} modbus_read_coil_status_query_t;
extern const BitField_T modbus_read_coil_status_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_coil_status_response_t;
extern const BitField_T modbus_read_coil_status_response_desc[];

typedef struct {
    uint16_t starting_address;
    uint16_t no_points;
} modbus_read_input_status_query_t;
extern const BitField_T modbus_read_input_status_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_input_status_response_t;
extern const BitField_T modbus_read_input_status_response_desc[];

typedef struct {
    uint16_t starting_address;
    uint16_t no_points;
} modbus_read_holding_registers_query_t;
extern const BitField_T modbus_read_holding_registers_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_holding_registers_response_t;
extern const BitField_T modbus_read_holding_registers_response_desc[];

typedef struct {
    uint16_t starting_address;
    uint16_t no_points;
} modbus_read_input_registers_query_t;
extern const BitField_T modbus_read_input_registers_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_input_registers_response_t;
extern const BitField_T modbus_read_input_registers_response_desc[];

typedef struct {
    uint16_t coil_address;
    uint16_t coil_data;
} modbus_force_single_coil_query_t;
extern const BitField_T modbus_force_single_coil_query_desc[];

typedef struct {
    uint16_t coil_address;
    uint16_t coil_data;
} modbus_force_single_coil_response_t;
extern const BitField_T modbus_force_single_coil_response_desc[];

typedef struct {
    uint16_t register_address;
    uint16_t preset_data;
} modbus_preset_single_register_query_t;
extern const BitField_T modbus_preset_single_register_query_desc[];

typedef struct {
    uint16_t register_address;
    uint16_t preset_data;
} modbus_preset_single_register_response_t;
extern const BitField_T modbus_preset_single_register_response_desc[];

typedef struct {
    uint8_t coil_data;
} modbus_read_exception_status_response_t;
extern const BitField_T modbus_read_exception_status_response_desc[];

typedef struct {
    uint16_t status;
    uint16_t event_count;
} modbus_fetch_comm_event_ctr_response_t;
extern const BitField_T modbus_fetch_comm_event_ctr_response_desc[];

typedef struct {
    size_t len;
    uint16_t status;
    uint16_t event_count;
    uint16_t message_count;
    uint8_t * events;
} modbus_fetch_comm_event_log_response_t;
extern const BitField_T modbus_fetch_comm_event_log_response_desc[];

typedef struct {
    uint16_t coil_address;
    uint16_t quantity_of_coils;
    size_t len;
    uint8_t * force_data;
} modbus_force_multiple_coils_query_t;
extern const BitField_T modbus_force_multiple_coils_query_desc[];

typedef struct {
    uint16_t coil_address;
    uint16_t quantity_of_coils;
} modbus_force_multiple_coils_response_t;
extern const BitField_T modbus_force_multiple_coils_response_desc[];

typedef struct {
    uint16_t starting_address;
    uint16_t no_registers;
    size_t len;
    uint8_t * data;
} modbus_preset_multiple_regs_query_t;
extern const BitField_T modbus_preset_multiple_regs_query_desc[];

typedef struct {
    uint16_t starting_address;
    uint16_t no_registers;
} modbus_preset_multiple_regs_response_t;
extern const BitField_T modbus_preset_multiple_regs_response_desc[];

typedef struct {
    size_t len;
    uint8_t slave_id;
    uint8_t run_indicator_status;
    uint8_t * additional_data;
} modbus_report_slave_id_response_t;
extern const BitField_T modbus_report_slave_id_response_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_general_reference_query_t;
extern const BitField_T modbus_read_general_reference_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_general_reference_response_t;
extern const BitField_T modbus_read_general_reference_response_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_write_general_reference_query_t;
extern const BitField_T modbus_write_general_reference_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_write_general_reference_response_t;
extern const BitField_T modbus_write_general_reference_response_desc[];

typedef struct {
    uint16_t reference_address;
    uint16_t and_mask;
    uint16_t or_mask;
} modbus_mask_write_4x_register_query_t;
extern const BitField_T modbus_mask_write_4x_register_query_desc[];

typedef struct {
    uint16_t reference_address;
    uint16_t and_mask;
    uint16_t or_mask;
} modbus_mask_write_4x_register_response_t;
extern const BitField_T modbus_mask_write_4x_register_response_desc[];

typedef struct {
    uint16_t read_reference_address;
    uint16_t quantity_to_read;
    uint16_t write_reference_address;
    uint16_t quantity_to_write;
    size_t len;
    uint8_t * write_data;
} modbus_read_write_4x_registers_query_t;
extern const BitField_T modbus_read_write_4x_registers_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_write_4x_registers_response_t;
extern const BitField_T modbus_read_write_4x_registers_response_desc[];

typedef struct {
    uint16_t fifo_pointer_address;
} modbus_read_fifo_queue_query_t;
extern const BitField_T modbus_read_fifo_queue_query_desc[];

typedef struct {
    size_t len;
    uint8_t * data;
} modbus_read_fifo_queue_response_t;
extern const BitField_T modbus_read_fifo_queue_response_desc[];

#endif //BITPARSER_MODBUS_H
