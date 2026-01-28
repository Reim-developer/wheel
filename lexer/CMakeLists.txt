add_library(lexer INTERFACE)

target_sources(
    lexer
    INTERFACE
        FILE_SET HEADERS 
        BASE_DIRS include
        FILES
            include/lexer/token.hxx
            include/lexer/cursor.hxx
            include/lexer/aliases.hxx
            include/lexer/handlers.hxx
            include/lexer/kind.hxx
            include/lexer/utils.hxx
            include/lexer/lexer.hxx
)

target_include_directories(
    lexer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)