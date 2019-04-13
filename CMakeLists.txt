cmake_minimum_required(VERSION 3.13)

PROJECT(BitParser C)

add_library(BitParser STATIC src/BitParser.c src/Stream.c src/UParser.c)