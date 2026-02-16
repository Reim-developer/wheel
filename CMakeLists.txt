cmake_minimum_required(VERSION 3.20)
project(wheel VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WHEEL_DEBUG)
    add_compile_definitions(WHEEL_DEBUG=ON)
endif()

if(WHEEL_ASSERTION)
    add_compile_definitions(WHEEL_ASSERTION=ON)
endif()

if(WHEEL_SOURCE_LOCATION)
    add_compile_definitions(WHEEL_SOURCE_LOCATION=ON)
endif()

if(WHEEL_EXPERIMENT)
    add_compile_definitions(WHEEL_EXPERIMENT=ON)
endif()

if(WHEEL_NO_FAIL_FAST)
    add_compile_definitions(WHEEL_NO_FAIL_FAST=ON)
endif()

if(WHEEL_TEST_BY_CONTEXT)
    add_compile_definitions(WHEEL_TEST_BY_CONTEXT=ON)
endif()

if(WHEEL_SMALL_VEC)
    add_compile_definitions(WHEEL_SMALL_VEC=ON)
endif()

add_subdirectory(wheel_utils)
add_subdirectory(analyzer)
add_subdirectory(wheel_memory)
add_subdirectory(wheel_lexer)
add_subdirectory(wheel_parser)

add_executable(
    ${PROJECT_NAME}
    src/main.cxx
)

enable_testing()
add_subdirectory(tests)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE wheel_lexer wheel_utils analyzer wheel_parser)