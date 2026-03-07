set(MODULE_NAME wheel_semantic)
add_library(${MODULE_NAME} STATIC)

target_sources(
    wheel_semantic
    PRIVATE
        semantic.cxx
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/wheel_semantic/program.hxx
            include/wheel_semantic/semantic.hxx
)

target_include_directories(
    wheel_semantic
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    wheel_semantic
    PUBLIC
        wheel_memory
        wheel_parser
        wheel_lexer
)
