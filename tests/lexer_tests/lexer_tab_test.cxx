#include "__lexer_test_helper/__helper.hxx"
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;

TEST(test_normal_tab)
    const char *source_code = "\t";

    Lexer lexer(source_code);
    const auto token = lexer.next_token();

    Verify<TokenKind::TAB>().token_metadata_eq_to(lexer.get_source_location(token), 1, 1, 0);
    Verify<TokenKind::TAB>().token_str_eq_to(token, "\t", 1);
    Verify<TokenKind::TAB>().token_as(token.kind);
DONE 

TEST(test_multiple_tab)
    const char *source_code = "\t\t\t";

    Lexer lexer(source_code);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();

    Verify<TokenKind::TAB>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);
    Verify<TokenKind::TAB>().token_str_eq_to(t1, "\t", 1);
    Verify<TokenKind::TAB>().token_as(t1.kind);

    Verify<TokenKind::TAB>().token_metadata_eq_to(lexer.get_source_location(t2), 1, 2, 1);
    Verify<TokenKind::TAB>().token_str_eq_to(t2, "\t", 1);
    Verify<TokenKind::TAB>().token_as(t2.kind);

    Verify<TokenKind::TAB>().token_metadata_eq_to(lexer.get_source_location(t3), 1, 3, 2);
    Verify<TokenKind::TAB>().token_str_eq_to(t3, "\t", 1);
    Verify<TokenKind::TAB>().token_as(t3.kind);
DONE 

TEST(test_mixed_tab_and_whitespace)
    const char *source = " \t";

    Lexer lexer(source);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();

    Verify<TokenKind::SPACE>().token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);
    Verify<TokenKind::SPACE>().token_as(t1.kind);
    Verify<TokenKind::SPACE>().token_str_eq_to(t1, " ", 1);

    Verify<TokenKind::TAB>().token_metadata_eq_to(lexer.get_source_location(t2), 1, 2, 1);
    Verify<TokenKind::TAB>().token_as(t2.kind);
    Verify<TokenKind::TAB>().token_str_eq_to(t2, "\t", 1);
DONE 

TEST_ENTRY
    RUN_TEST(test_normal_tab)
    RUN_TEST(test_multiple_tab)
    RUN_TEST(test_mixed_tab_and_whitespace)
ENTRY_END