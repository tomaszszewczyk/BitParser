add_library(unity STATIC unity.c)
target_include_directories(unity PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_custom_target(check COMMAND ctest --output-on-failure --progress WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

find_program(CMAKE_RUBY ruby)
if(NOT CMAKE_RUBY)
    message(FATAL_ERROR "Cannot find ruby.")
endif()

function(createTest TEST_NAME TEST_SRC)
    add_custom_command(OUTPUT ${TEST_NAME}_runner.c
            COMMAND ${CMAKE_RUBY} ${PROJECT_SOURCE_DIR}/test_framework/generate_test_runner.rb ${CMAKE_CURRENT_LIST_DIR}/${TEST_SRC} ${TEST_NAME}_runner.c
            DEPENDS ${TEST_SRC})

    list(REMOVE_AT ARGV 0)
    list(REMOVE_AT ARGV 0)

    add_executable(${TEST_NAME} ${TEST_SRC} ${TEST_NAME}_runner.c)
    target_link_libraries(${TEST_NAME} unity)
    target_link_libraries(${TEST_NAME} ${ARGV})
    add_test(${TEST_NAME} ${TEST_NAME})
    add_dependencies(check ${TEST_NAME})
endfunction()

