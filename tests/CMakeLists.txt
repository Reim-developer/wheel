add_executable(allocator_test allocator_test.cxx)
target_include_directories(allocator_test 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)
set_target_properties(allocator_test PROPERTIES EXCLUDE_FROM_ALL ON)
set_target_properties(allocator_test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)

add_executable(int_type_test 
    type_tests/int_type_test.cxx
    ../src/ast/types/int_type.cxx
)
target_include_directories(int_type_test 
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)
set_target_properties(int_type_test  PROPERTIES EXCLUDE_FROM_ALL ON)
set_target_properties(int_type_test  PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)

add_executable(int_grammar_test grammar_tests/int_grammar_test.cxx)
target_include_directories(int_grammar_test
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/vendor
)
set_target_properties(int_grammar_test  PROPERTIES EXCLUDE_FROM_ALL ON)
set_target_properties(int_grammar_test PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)

add_test(NAME AllocatorTest  COMMAND allocator_test)
add_test(NAME IntTypeTest    COMMAND int_type_test)
add_test(NAME IntGrammarTest COMMAND int_grammar_test)