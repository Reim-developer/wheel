setup_wheel_test(lexer_eof_test lexer_eof_test.cxx)

target_link_libraries(lexer_eof_test PRIVATE lexer)
set_property(TARGET lexer_eof_test PROPERTY CXX_STANDARD 20)