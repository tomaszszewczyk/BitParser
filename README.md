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

## UParser

## BitParser