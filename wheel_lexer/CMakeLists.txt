add_library(wheel_lexer INTERFACE)

target_sources(
    wheel_lexer
    INTERFACE
        FILE_SET HEADERS 
        BASE_DIRS lib
        FILES
            lib/token.hxx
            lib/cursor.hxx
            lib/aliases.hxx
            lib/handlers.hxx
            lib/kind.hxx
            lib/utils.hxx
            lib/lexer.hxx
)

target_include_directories(
    wheel_lexer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)