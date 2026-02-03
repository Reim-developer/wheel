add_library(wheel_lexer INTERFACE)

target_sources(
    wheel_lexer
    INTERFACE
        FILE_SET HEADERS 
        BASE_DIRS include
        FILES
            include/wheel_lexer/token.hxx
            include/wheel_lexer/cursor.hxx
            include/wheel_lexer/aliases.hxx
            include/wheel_lexer/handlers.hxx
            include/wheel_lexer/kind.hxx
            include/wheel_lexer/utils.hxx
            include/wheel_lexer/lexer.hxx
)

target_include_directories(
    wheel_lexer
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)