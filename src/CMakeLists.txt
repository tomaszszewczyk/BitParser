add_library(BitParser STATIC BitParser.c Stream.c UParser.c)
target_include_directories(BitParser PUBLIC ${CMAKE_CURRENT_LIST_DIR})