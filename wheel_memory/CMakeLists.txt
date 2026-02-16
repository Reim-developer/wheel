add_library(wheel_memory INTERFACE)

target_sources(
    wheel_memory
    INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
        include/wheel_memory/vec.hxx
)

target_include_directories(
    wheel_memory
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
