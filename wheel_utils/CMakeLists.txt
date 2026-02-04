add_library(wheel_utils INTERFACE)

target_sources(
    wheel_utils
    INTERFACE
        FILE_SET HEADERS 
        BASE_DIRS include
        FILES
           include/logging.hxx
)

target_include_directories(
    wheel_utils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)