# BitParser

![Travis]( https://travis-ci.com/tomaszszewczyk/BitParser.svg?branch=master "Travis")

## Overview

Nobody likes writing parsers. I hate it so much I decided to write one last parser in my life. That is how BitParser was
born. BitParser is a simple library wrapping all boring, tedious and error prone stuff that have to be done in order
to parse complicated binary encoded data. It handles for you endianness changes, different bit sizes, data length,
two fashions of signed integers and provide simple engine for parsing messages using memory condensed descriptors.
BitParser consist of three main elements, which provides three layers of abstraction. These elements are: Stream,
UParser and BitParser, where Stream is most low level, UParser uses Stream and BitParser uses UParser. Each module can
be used without higher level modules if desired. Library is written to be suitable for embedded development, so
there is no dynamic allocation and other expensive operations. Here is some more detailed description of BitParser lib.

## Stream

Stream is a simple object describing a piece of memory or an array. It contains of a pointer to where the actual data is
stored, whole memory len and index pointing to current Stream position. Stream has two endianness modes LITTLE and BIG.
Stream takes a lot of responsibility from you. It takes care of buffer len not allowing you to write or read more data
than you are supposed to, it handles different endianness modes saving a lot of headache and it takes care of bit
shifts or glueing bits into bytes. It mimics C standard library stream API to make it as easy to use as possible.
Finally there are to classes of functions one with suffix `Bit` and the other without. Bit functions let you operate on
bit level, ie. they let you read only one bit from a buffer and other functions operates on byte level. If you don't
need bit level functionality you can use byte level functions letting the compiler to optimize away flash expensive
binary operations, which is important in embedded development. Byte level functions are much faster too.

Here is simple example of Stream usage:
```c
#include "Stream.h"

int main() {
    uint8_t data1 = 0xAA, data2 = 0xBB, data3 = 0xCC, data4 = 0xDD;
    uint8_t message[3] = {0};
    Stream_T stream;

    Stream_Init(&stream, message, sizeof(message), LITTLE);
    Stream_WriteBit(&stream, &data1, 4);
    Stream_SeekBit(&stream, 8);
    Stream_WriteBit(&stream, &data2, 4);
    Stream_WriteBit(&stream, &data3, 4);
    Stream_WriteBit(&stream, &data4, 4);

    printf("0x%02X 0x%02X 0x%02X\n", message[0], message[1], message[2]);
}
```
```
$ ./example_stream
0x0A 0xCB 0x0D
```
First, we create some variables with data to be written to Stream, then some space in memory for our serialized message.
Then Stream kicks in. We write 4 bit of variable `data1`, then we move cursor to 8th bit in Stream, then and then we
write 4 bits of `data2`, `data3` and `data4`. The output is obviously `0x0A 0xCB 0x0D`. How easy was that!
No struct packing, no bitwise operations. Please also notice how easy it is to change endianness.

## UParser

The next level of abstraction is UParser, which can be used for manual (but way simpler than ever) parser writing.
It implements methods for writing and reading concrete types, including unsigned integers
signed integers, floating point types, `size_t` and arrays. It even implements both two's complements and
sign and magnitude representation integer representation. It's all there! Again there are both bit and byte level
functions so you pay only for what you use. UParser module use Stream so you get all features from Stream, including
easy endianness swapping.

Here is some example of UParser in action:
```c
#include "UParser.h"

int main() {
    uint32_t a = 12654;
    int32_t  b = -13543;
    float    c = 0.5;

    uint8_t msg[13];
    Stream_T stream;

    Stream_Init(&stream, msg, sizeof(msg), BIG);
    U32_SerializeBit(&a, 24, &stream);
    I32_SerializeBit(&b, 24, &stream);
    S32_SerializeBit(&b, 24, &stream);
    Float_SerializeBit(&c, &stream);

    for(size_t i = 0; i < sizeof(msg); i++)
        printf("0x%02X ", msg[i]);
    printf("\n");
}
```
```
$ ./example_uparser
0x00 0x31 0x6E 0xFF 0xCB 0x19 0x80 0x34 0xE7 0x3F 0x00 0x00 0x00
```
Again we create some data we wish to serialize, then buffer and stream itself. We insert created data into stream,
but with the width of 3 bytes. Notice how it is possible to insert signed data with two different encodings.

## BitParser

On the top of these come BitParser, the module that allows you to leverage the advantages of whole lib and don't
spend time on writing all this boring stuff by hand. You just need to define a struct representing message in memory
and provide a description of serialized message format. The result is condensed in code and in memory! A message
description takes only 12B of flash memory per field. Here is how you write the above with BitParser:
```c
#include "BitParser.h"

typedef struct {
    uint32_t a;
    int32_t  b;
    float    c;
} MyMsg_T;

static const BitField_T my_msg_desc[] = {
    BIT_FIELD_U32(24, MyMsg_T, a),
    BIT_FIELD_I32(24, MyMsg_T, b),
    BIT_FIELD_S32(24, MyMsg_T, b),
    BIT_FIELD_FLOAT(MyMsg_T, c),
};

int main() {
    MyMsg_T data = {.a = 12654, .b = -13543, .c = 0.5};
    uint8_t msg[13] = {0};

    Stream_T stream;
    Stream_Init(&stream, msg, sizeof(msg), BIG);

    Status_T ret = BitParser_Serialize(my_msg_desc, ARRAY_LEN(my_msg_desc), &data, &stream);
    if(ret != STATUS_SUCCESS)
        return 1;

    for(size_t i = 0; i < sizeof(msg); i++)
        printf("0x%02X ", msg[i]);
    printf("\n");
}
```

## Error handling

All functions in BitParser library are written with care for error handling. They return a value to notify that
something went wrong, so it is very easy to make assertions and be sure everything is OK. Here is how you use it:
```c
#include "Stream.h"

int main() {
    uint8_t data1 = 0xAA, data2 = 0xBB, data3 = 0xCC, data4 = 0xDD;
    uint8_t message[3] = {0};
    Stream_T stream;

    Status_T ret;
    Stream_Init(&stream, message, sizeof(message), LITTLE);

    ret = Stream_WriteBit(&stream, &data1, 4);
    if(ret != STATUS_SUCCESS)
        return 1;

    ret = Stream_SeekBit(&stream, 8);
    if(ret != STATUS_SUCCESS)
        return 1;

    ret = Stream_WriteBit(&stream, &data2, 4);
    if(ret != STATUS_SUCCESS)
        return 1;

    ret = Stream_WriteBit(&stream, &data3, 4);
    if(ret != STATUS_SUCCESS)
        return 1;

    ret = Stream_WriteBit(&stream, &data4, 4);
    if(ret != STATUS_SUCCESS)
        return 1;

    printf("0x%02X 0x%02X 0x%02X\n", message[0], message[1], message[2]);
}
```

## How to build this project?

To build this project you need to do the following:
```bash
$ mkdir build && cd build   # To create directory for your binary tree
$ cmake ..                  # To configure project from cmake
$ make && make check        # To build lib, examples and run tests
```
Also the project depends on CMake and Ruby.

## Contributing

The only accepted kind of criticism here are pull requests, so feel free to add something from yourself ;)