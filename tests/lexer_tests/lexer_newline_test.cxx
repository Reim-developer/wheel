#include "__lexer_test_helper/__helper.hxx"
#include <__internal/__assert.hxx>
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::Kind;

TEST(test_lf_newline) 
    const char* source = "\n";
    Lexer lexer(source);
    const auto token  = lexer.next_token();

    Verify<Kind::NEWLINE>().token_str_eq_to(token, "\n", 1);
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(token), 1, 1, 0);
DONE 

TEST(test_crlf_newline)
    const char* source = "\r\n";
    Lexer lexer(source);
    const auto token  = lexer.next_token();

    Verify<Kind::NEWLINE>().token_str_eq_to(token, "\r\n", 2);
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(token), 1, 1, 0);
DONE

TEST(test_cr_newline)
    const char* source = "\r";
    Lexer lexer(source);
    const auto token  = lexer.next_token();

    Verify<Kind::NEWLINE>().token_str_eq_to(token, "\r", 1);
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(token), 1, 1, 0);
DONE

TEST(test_multilpe_new_lines)
    const char* source = "\n\n\n";
    Lexer lexer(source);
    
    auto t1 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);
    
    auto t2 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t2), 2, 1, 1);

    auto t3 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t3), 3, 1, 2);
DONE 

TEST(test_multilpe_new_lines_clrf)
    const char* source = "\r\n\r\n\r\n";
    Lexer lexer(source);
    
    auto t1 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);
    
    auto t2 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t2), 2, 2, 2);

    auto t3 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t3), 3, 2, 4);
DONE

TEST(test_multilpe_new_cr)
    const char* source = "\r\r\r";
    Lexer lexer(source);

    const auto t1  = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);

    const auto t2 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t2), 2, 1, 1);

    const auto t3 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t3), 3, 1, 2);
DONE

TEST(test_multilpe_new_line_mixed)
    const char* source = "\n\r\r\n";
    Lexer lexer(source);

    const auto t1  = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);

    const auto t2 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t2), 2, 1, 1);

    const auto t3 = lexer.next_token();
    Verify<Kind::NEWLINE>().token_metadata_eq_to(lexer.get_source_location(t3), 3, 1, 2);
DONE 

TEST_ENTRY
    RUN_TEST(test_lf_newline)
    RUN_TEST(test_crlf_newline)
    RUN_TEST(test_cr_newline)
    RUN_TEST(test_multilpe_new_lines)
    RUN_TEST(test_multilpe_new_lines_clrf)
    RUN_TEST(test_multilpe_new_cr)
    RUN_TEST(test_multilpe_new_line_mixed);
ENTRY_END
