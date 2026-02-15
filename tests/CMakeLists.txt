function (setup_wheel_test test_name test_source)
    if(ARGN)
        message("Found addtional sources ${ARGN}")
        set(addtional_sources ${ARGN})
        add_executable(${test_name} ${test_source} ${addtional_sources})
    else()
        add_executable(${test_name} ${test_source})
    endif()

    target_include_directories(
        ${test_name}
        PRIVATE
            ${CMAKE_SOURCE_DIR}/tests  
            ${CMAKE_SOURCE_DIR}/wheel_lexer
            ${CMAKE_SOURCE_DIR}/wheel_utils
            ${CMAKE_SOURCE_DIR}/wheel_parser
    )

    target_link_libraries(${test_name} PRIVATE wheel_lexer)
    target_link_libraries(${test_name} PRIVATE wheel_utils)
    target_link_libraries(${test_name} PRIVATE wheel_parser)
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 20)

    set_target_properties(
        ${test_name} PROPERTIES EXCLUDE_FROM_ALL ON)
    set_target_properties(
        ${test_name} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_subdirectory(lexer_tests)
add_subdirectory(analyzer_tests)
add_subdirectory(parser_tests)
add_subdirectory(utils_tests)