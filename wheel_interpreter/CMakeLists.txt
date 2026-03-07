add_library(wheel_interpreter STATIC)

target_sources(
    wheel_interpreter
    PRIVATE
        interpreter.cxx
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/wheel_interpreter/interpreter.hxx
)

target_include_directories(
    wheel_interpreter
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    wheel_interpreter
    PUBLIC
        wheel_memory
        wheel_semantic
)
