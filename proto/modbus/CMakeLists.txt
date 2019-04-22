add_library(modbus STATIC modbus.c modbus.h)
target_link_libraries(modbus BitParser)