add_library(wheel_parser STATIC)

target_sources(wheel_parser
    PRIVATE 
        parser.cxx
        nodes.cxx

    PUBLIC
        FILE_SET    HEADERS
        BASE_DIRS   include
        FILES
            include/wheel_parser/config.hxx
            include/wheel_parser/error.hxx
            include/wheel_parser/parser.hxx
            include/wheel_parser/parser_utils.hxx
            include/wheel_parser/functions/variables_declaration.hxx
            include/wheel_parser/ast/symbol.hxx
            include/wheel_parser/ast/builtins.hxx
            include/wheel_parser/ast/nodes.hxx
            include/wheel_parser/ast/keywords.hxx
)

target_include_directories(
    wheel_parser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)   

target_link_libraries(wheel_parser PUBLIC wheel_memory wheel_utils wheel_lexer)
