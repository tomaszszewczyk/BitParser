# BitParser

![Travis]( https://travis-ci.com/tomaszszewczyk/BitParser.svg?branch=master "Travis")

## Overview

Nobody likes writing parsers. I hate it so much I decided to write one last parser in my life. That is how BitParser was
born. BitParser is a simple library wrapping all boring, tedious and error prone stuff that have to be done in order 
to parse complicated binary encoded data. It handles for you endianness changes, different bit sizes, data length, 
two fashions of signed integers and provide simple engine for parsing messages using memory condensed descriptors.
BitParser consist of three main elements, which provides three layers of abstraction. These elements are: Stream,
UParser and BitParser, where Stream is most low level, UParser uses Stream and BitParser uses UParser. Each module can
be used without higher level modules if desired. Here is some more detailed description of BitParser lib.

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

Here is simple example of Stream:
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
First, we create some variables with data to be written to Stream, then some space in memory for our serialized message.
Then Stream kicks in. We write 4 bit of variable `data1`, then we move cursor to 8th bit in Stream, then and then we 
write 4 bits of `data2`, `data3` and `data4`. The output is obviously `0x0A 0xCB 0x0D`. How easy was that!
No struct packing, no bitwise operations. Please also notice how easy it is to change endianness.

## UParser

## BitParser