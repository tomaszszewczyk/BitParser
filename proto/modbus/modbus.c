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

#include "modbus.h"

const BitField_T modbus_read_coil_status_query[] = {
        BIT_FIELD_U16(16, modbus_read_coil_status_query_t, starting_address),
        BIT_FIELD_U16(16, modbus_read_coil_status_query_t, no_points),
};

const BitField_T modbus_read_coil_status_response[] = {
        BIT_FIELD_LEN(8, modbus_read_coil_status_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_coil_status_response_t, data, len),
};

const BitField_T modbus_read_input_status_query[] = {
        BIT_FIELD_U16(16, modbus_read_input_status_query_t, starting_address),
        BIT_FIELD_U16(16, modbus_read_input_status_query_t, no_points),
};

const BitField_T modbus_read_input_status_response[] = {
        BIT_FIELD_LEN(8, modbus_read_input_status_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_input_status_response_t, data, len),
};

const BitField_T modbus_read_holding_registers_query[] = {
        BIT_FIELD_U16(16, modbus_read_holding_registers_query_t, starting_address),
        BIT_FIELD_U16(16, modbus_read_holding_registers_query_t, no_points),
};

const BitField_T modbus_read_holding_registers_response[] = {
        BIT_FIELD_LEN(8, modbus_read_holding_registers_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_holding_registers_response_t, data, len),
};

const BitField_T modbus_read_input_registers_query[] = {
        BIT_FIELD_U16(16, modbus_read_input_registers_query_t, starting_address),
        BIT_FIELD_U16(16, modbus_read_input_registers_query_t, no_points),
};

const BitField_T modbus_read_input_registers_response[] = {
        BIT_FIELD_LEN(8, modbus_read_input_registers_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_input_registers_response_t, data, len),
};

const BitField_T modbus_force_single_coil_query[] = {
        BIT_FIELD_U16(16, modbus_force_single_coil_query_t, coil_address),
        BIT_FIELD_U16(16, modbus_force_single_coil_query_t, coil_data),
};

const BitField_T modbus_force_single_coil_response[] = {
        BIT_FIELD_U16(16, modbus_force_single_coil_response_t, coil_address),
        BIT_FIELD_U16(16, modbus_force_single_coil_response_t, coil_data),
};

const BitField_T modbus_preset_single_register_query[] = {
        BIT_FIELD_U16(16, modbus_preset_single_register_query_t, register_address),
        BIT_FIELD_U16(16, modbus_preset_single_register_query_t, preset_data),
};

const BitField_T modbus_preset_single_register_response[] = {
        BIT_FIELD_U16(16, modbus_preset_single_register_response_t, register_address),
        BIT_FIELD_U16(16, modbus_preset_single_register_response_t, preset_data),
};

const BitField_T modbus_read_exception_status_response[] = {
        BIT_FIELD_U8(8, modbus_read_exception_status_response_t, coil_data),
};

const BitField_T modbus_fetch_comm_event_ctr_response[] = {
        BIT_FIELD_U16(16, modbus_fetch_comm_event_ctr_response_t, status),
        BIT_FIELD_U16(16, modbus_fetch_comm_event_ctr_response_t, event_count),
};

const BitField_T modbus_fetch_comm_event_log_response[] = {
        BIT_FIELD_LEN(8, modbus_fetch_comm_event_log_response_t, len),
        BIT_FIELD_U16(16, modbus_fetch_comm_event_log_response_t, status),
        BIT_FIELD_U16(16, modbus_fetch_comm_event_log_response_t, event_count),
        BIT_FIELD_U16(16, modbus_fetch_comm_event_log_response_t, message_count),
        BIT_FIELD_ARRAY_VARIABLE_WHOLE_MSG(modbus_fetch_comm_event_log_response_t, events, len),
};

const BitField_T modbus_force_multiple_coils_query[] = {
        BIT_FIELD_U16(16, modbus_force_multiple_coils_query_t, coil_address),
        BIT_FIELD_U16(16, modbus_force_multiple_coils_query_t, quantity_of_coils),
        BIT_FIELD_LEN(8, modbus_force_multiple_coils_query_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_force_multiple_coils_query_t, force_data, len),
};

const BitField_T modbus_force_multiple_coils_response[] = {
        BIT_FIELD_U16(16, modbus_force_multiple_coils_response_t, coil_address),
        BIT_FIELD_U16(16, modbus_force_multiple_coils_response_t, quantity_of_coils),
};

const BitField_T modbus_preset_multiple_regs_query[] = {
        BIT_FIELD_U16(16, modbus_preset_multiple_regs_query_t, starting_address),
        BIT_FIELD_U16(16, modbus_preset_multiple_regs_query_t, no_registers),
        BIT_FIELD_LEN(8, modbus_preset_multiple_regs_query_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_preset_multiple_regs_query_t, data, len),
};

const BitField_T modbus_preset_multiple_regs_response[] = {
        BIT_FIELD_U16(16, modbus_preset_multiple_regs_response_t, starting_address),
        BIT_FIELD_U16(16, modbus_preset_multiple_regs_response_t, no_registers),
};

const BitField_T modbus_report_slave_id_response[] = {
        BIT_FIELD_LEN(8, modbus_report_slave_id_response_t, len),
        BIT_FIELD_U8(8, modbus_report_slave_id_response_t, slave_id),
        BIT_FIELD_U8(8, modbus_report_slave_id_response_t, run_indicator_status),
        BIT_FIELD_ARRAY_VARIABLE_WHOLE_MSG(modbus_report_slave_id_response_t, additional_data, len),
};

const BitField_T modbus_read_general_reference_query[] = {
        BIT_FIELD_LEN(8, modbus_read_general_reference_query_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_general_reference_query_t, data, len),
};

const BitField_T modbus_read_general_reference_response[] = {
        BIT_FIELD_LEN(8, modbus_read_general_reference_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_general_reference_response_t, data, len),
};

const BitField_T modbus_write_general_reference_query[] = {
        BIT_FIELD_LEN(8, modbus_write_general_reference_query_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_write_general_reference_query_t, data, len),
};

const BitField_T modbus_write_general_reference_response[] = {
        BIT_FIELD_LEN(8, modbus_write_general_reference_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_write_general_reference_response_t, data, len),
};

const BitField_T modbus_mask_write_4x_register_query[] = {
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_query_t, reference_address),
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_query_t, and_mask),
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_query_t, or_mask),
};

const BitField_T modbus_mask_write_4x_register_response[] = {
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_response_t, reference_address),
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_response_t, and_mask),
        BIT_FIELD_U16(16, modbus_mask_write_4x_register_response_t, or_mask),
};

const BitField_T modbus_read_write_4x_registers_query[] = {
        BIT_FIELD_U16(16, modbus_read_write_4x_registers_query_t, read_reference_address),
        BIT_FIELD_U16(16, modbus_read_write_4x_registers_query_t, quantity_to_read),
        BIT_FIELD_U16(16, modbus_read_write_4x_registers_query_t, write_reference_address),
        BIT_FIELD_U16(16, modbus_read_write_4x_registers_query_t, quantity_to_write),
        BIT_FIELD_LEN(8, modbus_read_write_4x_registers_query_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_write_4x_registers_query_t, write_data, len),
};

const BitField_T modbus_read_write_4x_registers_response[] = {
        BIT_FIELD_LEN(8, modbus_read_write_4x_registers_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_write_4x_registers_response_t, data, len),
};

const BitField_T modbus_read_fifo_queue_query[] = {
        BIT_FIELD_U16(16, modbus_read_fifo_queue_query_t, fifo_pointer_address),
};

const BitField_T modbus_read_fifo_queue_response[] = {
        BIT_FIELD_LEN(8, modbus_read_fifo_queue_response_t, len),
        BIT_FIELD_ARRAY_VARIABLE(modbus_read_fifo_queue_response_t, data, len),
};
