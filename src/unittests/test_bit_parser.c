#include "unity.h"

#include "BitParser.c"
#include "UParser.c"
#include "Stream.c"
#include "BitParserError.h"

void test_simple(void) {
    // Given
    typedef struct {
        uint8_t aaa;
        uint8_t bbb;
    } Msg_T;

    static const BitField_T msg_desc[] = {
            BIT_FIELD_U8(4, Msg_T, aaa),
            BIT_FIELD_U8(4, Msg_T, bbb),
    };

    Msg_T msg = {
            .aaa = 0xAB,
            .bbb = 0xCD,
    };

    //When
    size_t len = BitParser_GetLength(msg_desc, ARRAY_LEN(msg_desc), &msg);
    uint8_t output[len];
    memset(output, 0, sizeof(output));
    Stream_T stream;
    Stream_Init(&stream, output, len, LITTLE);
    Status_T result = BitParser_Serialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    //Then
    uint8_t expected[] = {0xDB};
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

void test_simple_array(void) {
    // Given
    typedef struct {
        size_t    aaa;
        uint8_t * bbb;
    } Msg_T;

    static const BitField_T msg_desc[] = {
        BIT_FIELD_LEN(8, Msg_T, aaa),
        BIT_FIELD_ARRAY_VARIABLE(Msg_T, bbb, aaa),
    };

    uint8_t arr[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    Msg_T msg = {
        .aaa = 8,
        .bbb = arr,
    };

    //When
    size_t len = BitParser_GetLength(msg_desc, ARRAY_LEN(msg_desc), &msg);
    uint8_t output[len];
    memset(output, 0, sizeof(output));
    Stream_T stream;
    Stream_Init(&stream, output, len, LITTLE);
    Status_T result = BitParser_Serialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    //Then
    uint8_t expected[] = {0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

void test_simple_align(void) {
    // Given
    typedef struct {
        uint8_t aaa;
        uint8_t bbb;
    } Msg_T;

    static const BitField_T msg_desc[] = {
        BIT_FIELD_U8(4, Msg_T, aaa),
        BIT_FIELD_ALIGN(),
        BIT_FIELD_U8(4, Msg_T, bbb),
    };

    Msg_T msg = {
        .aaa = 0xAB,
        .bbb = 0xCD,
    };

    //When
    size_t len = BitParser_GetLength(msg_desc, ARRAY_LEN(msg_desc), &msg);
    uint8_t output[len];
    memset(output, 0, sizeof(output));
    Stream_T stream;
    Stream_Init(&stream, output, len, LITTLE);
    Status_T result = BitParser_Serialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    //Then
    uint8_t expected[] = {0x0B, 0x0D};
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

void test_simple_pad(void) {
    // Given
    typedef struct {
        uint8_t aaa;
        uint8_t bbb;
    } Msg_T;

    static const BitField_T msg_desc[] = {
        BIT_FIELD_U8(4, Msg_T, aaa),
        BIT_FIELD_PAD(16),
        BIT_FIELD_U8(4, Msg_T, bbb),
    };

    Msg_T msg = {
        .aaa = 0xAB,
        .bbb = 0xCD,
    };

    //When
    size_t len = BitParser_GetLength(msg_desc, ARRAY_LEN(msg_desc), &msg);
    uint8_t output[len];
    memset(output, 0, sizeof(output));
    Stream_T stream;
    Stream_Init(&stream, output, len, LITTLE);
    Status_T result = BitParser_Serialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    //Then
    uint8_t expected[] = {0x0B, 0x00, 0xD0};
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

void test_modbus_read_input_registers(void) {
    // Given
    typedef struct {
        uint8_t slave_address;
        uint8_t function;
        uint16_t address;
        uint16_t number_of_points;
        uint16_t error_check;
    } ReadInputRegister_T;

    static const BitField_T msg_desc[] = {
        BIT_FIELD_U8(8, ReadInputRegister_T, slave_address),
        BIT_FIELD_U8(8, ReadInputRegister_T, function),
        BIT_FIELD_U16(16, ReadInputRegister_T, address),
        BIT_FIELD_U16(16, ReadInputRegister_T, number_of_points),
        BIT_FIELD_U16(16, ReadInputRegister_T, error_check),
    };

    ReadInputRegister_T msg = {
        .slave_address =    0x01,
        .function =         0x04,
        .address =          0x0000,
        .number_of_points = 0x0002,
        .error_check =      0xCB71,
    };

    //When
    size_t len = BitParser_GetLength(msg_desc, ARRAY_LEN(msg_desc), &msg);
    uint8_t output[len];
    memset(output, 0, sizeof(output));
    Stream_T stream;
    Stream_Init(&stream, output, len, BIG);
    Status_T result = BitParser_Serialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    //Then
    uint8_t expected[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0xCB, 0x71};
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

void test_modbus_deserialize(void) {
    // Given
    typedef struct {
        uint8_t slave_address;
        uint8_t function;
        uint16_t address;
        uint16_t number_of_points;
        uint16_t error_check;
    } ReadInputRegister_T;

    static const BitField_T msg_desc[] = {
        BIT_FIELD_U8(8, ReadInputRegister_T, slave_address),
        BIT_FIELD_U8(8, ReadInputRegister_T, function),
        BIT_FIELD_U16(16, ReadInputRegister_T, address),
        BIT_FIELD_U16(16, ReadInputRegister_T, number_of_points),
        BIT_FIELD_U16(16, ReadInputRegister_T, error_check),
    };

    //When
    uint8_t input[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0xCB, 0x71};
    Stream_T stream;
    Stream_Init(&stream, input, sizeof(input), BIG);
    ReadInputRegister_T msg = {0};
    Status_T result = BitParser_Deserialize(msg_desc, ARRAY_LEN(msg_desc), &msg, &stream);

    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
    TEST_ASSERT_EQUAL(0x01, msg.slave_address);
    TEST_ASSERT_EQUAL(0x04, msg.function);
    TEST_ASSERT_EQUAL(0x0000, msg.address);
    TEST_ASSERT_EQUAL(0x0002, msg.number_of_points);
    TEST_ASSERT_EQUAL(0xCB71, msg.error_check);
}

void test_misc_decode(void) {
    //Given
    uint8_t input[] = {
        0b01011100,
        0b01111010,
        0b11101110,
        0b10001000,
        0b01111101,
        0b00010000,
        0b00100011,
        0b00110111,
        0b10000010,
        0b11111010,
        0b01011011,
        0b00101110,
        0b10011101,
        0b00011010,
    };

    typedef struct {
        uint32_t time;
        uint16_t sensor1;
        uint16_t sensor2;
        uint16_t sensor3;
        uint16_t sensor4;
        uint8_t  binary_sensor1;
        uint8_t  binary_sensor2;
        uint8_t  sensor1_type;
        uint8_t  sensor2_type;
        uint8_t  sensor3_type;
        uint8_t  sensor4_type;
        uint8_t  binary_sensor1_config;
        uint8_t  binary_sensor2_config;
        uint8_t  alarm1;
        uint8_t  alarm2;
        uint8_t  alarm3;
        uint8_t  alarm4;
        uint8_t  relay;
        uint8_t  flags;
    } Misc_T;

    static const BitField_T msg_dsc[] = {
        BIT_FIELD_U32(32, Misc_T, time),
        BIT_FIELD_U16(12, Misc_T, sensor1),
        BIT_FIELD_U16(12, Misc_T, sensor2),
        BIT_FIELD_U16(12, Misc_T, sensor3),
        BIT_FIELD_U16(12, Misc_T, sensor4),
        BIT_FIELD_U8(1, Misc_T, binary_sensor1),
        BIT_FIELD_U8(1, Misc_T, binary_sensor2),
        BIT_FIELD_U8(2, Misc_T, sensor1_type),
        BIT_FIELD_U8(2, Misc_T, sensor2_type),
        BIT_FIELD_U8(2, Misc_T, sensor3_type),
        BIT_FIELD_U8(2, Misc_T, sensor4_type),
        BIT_FIELD_U8(1, Misc_T, binary_sensor1_config),
        BIT_FIELD_U8(1, Misc_T, binary_sensor2_config),
        BIT_FIELD_U8(3, Misc_T, alarm1),
        BIT_FIELD_U8(3, Misc_T, alarm2),
        BIT_FIELD_U8(3, Misc_T, alarm3),
        BIT_FIELD_U8(3, Misc_T, alarm4),
        BIT_FIELD_U8(1, Misc_T, relay),
        BIT_FIELD_U8(4, Misc_T, flags),
    };

    //When
    Stream_T stream;
    Stream_Init(&stream, input, sizeof(input), BIG);
    Misc_T output;
    Status_T result = BitParser_Deserialize(msg_dsc, ARRAY_LEN(msg_dsc), &output, &stream);

    //Then
    TEST_ASSERT_EQUAL(output.time, 0x5C7AEE88);
    TEST_ASSERT_EQUAL(output.sensor1, 0x07D1);
    TEST_ASSERT_EQUAL(output.sensor2, 0x0023);
    TEST_ASSERT_EQUAL(output.sensor3, 0x0378);
    TEST_ASSERT_EQUAL(output.sensor4, 0x02FA);
    TEST_ASSERT_EQUAL(output.binary_sensor1, 0x0000);
    TEST_ASSERT_EQUAL(output.binary_sensor2, 0x0001);
    TEST_ASSERT_EQUAL(output.sensor1_type, 0x0001);
    TEST_ASSERT_EQUAL(output.sensor2_type, 0x0002);
    TEST_ASSERT_EQUAL(output.sensor3_type, 0x0003);
    TEST_ASSERT_EQUAL(output.sensor4_type, 0x0000);
    TEST_ASSERT_EQUAL(output.binary_sensor1_config, 0x0001);
    TEST_ASSERT_EQUAL(output.binary_sensor2_config, 0x0000);
    TEST_ASSERT_EQUAL(output.alarm1, 0x0007);
    TEST_ASSERT_EQUAL(output.alarm2, 0x0002);
    TEST_ASSERT_EQUAL(output.alarm3, 0x0003);
    TEST_ASSERT_EQUAL(output.alarm4, 0x0005);
    TEST_ASSERT_EQUAL(output.relay, 0x0000);
    TEST_ASSERT_EQUAL(output.flags, 0x0003);
    TEST_ASSERT_EQUAL(STATUS_SUCCESS, result);
}
