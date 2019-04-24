add_executable(example_stream stream.c)
target_link_libraries(example_stream BitParser)

add_executable(example_uparser uparser.c)
target_link_libraries(example_uparser BitParser)

add_executable(example_bitparser bitparser.c)
target_link_libraries(example_bitparser BitParser)