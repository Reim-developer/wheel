add_library(analyzer STATIC)

target_sources(analyzer
    PRIVATE
        src/tab.cxx
    
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/analyzer/tab.hxx
)

target_include_directories(
    analyzer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)