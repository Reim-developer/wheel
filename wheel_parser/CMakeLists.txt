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
            include/wheel_parser/allocator.hxx
            include/wheel_parser/parser.hxx
            include/wheel_parser/ast/symbol.hxx
            include/wheel_parser/ast/nodes.hxx
)

target_include_directories(
    wheel_parser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)   

target_link_libraries(wheel_parser PUBLIC wheel_utils wheel_lexer)