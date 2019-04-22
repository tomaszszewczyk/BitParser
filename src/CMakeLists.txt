add_library(BitParser STATIC BitParser.c Stream.c UParser.c ../proto/modbus/modbus.h ../proto/modbus/modbus.c)
target_include_directories(BitParser PUBLIC ${CMAKE_CURRENT_LIST_DIR})