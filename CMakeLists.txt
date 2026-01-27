cmake_minimum_required(VERSION 3.20)
project(wheel VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(analyzer)
add_executable(
    ${PROJECT_NAME}
    src/main.cxx
    src/ast/nodes/node_ast.cxx
)

enable_testing()
add_subdirectory(tests)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/src
)

target_link_directories(${PROJECT_NAME} PRIVATE analyzer)
