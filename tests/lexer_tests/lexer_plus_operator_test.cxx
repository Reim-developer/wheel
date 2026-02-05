#include "__lexer_test_helper/__helper.hxx"
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;

TEST(test_normal_plus_operator)
    const char *source = "+";
    Lexer lexer(source);

    auto token = lexer.next_token();
    VERIFY_THIS(TokenKind::PLUS).token_str_eq_to(token, "+", 1);
    VERIFY_THIS(TokenKind::PLUS).token_metadata_eq_to(token, 1, 2, 0);
    VERIFY_THIS(TokenKind::PLUS).token_as(token.kind);
DONE 

TEST(test_normal_plus_plus_operator)
    const char *source = "++";
    Lexer lexer(source);

    auto token = lexer.next_token();
    VERIFY_THIS(TokenKind::PLUS_PLUS).token_str_eq_to(token, "++", 2);
    VERIFY_THIS(TokenKind::PLUS_PLUS).token_metadata_eq_to(token, 1, 3, 0);
    VERIFY_THIS(TokenKind::PLUS_PLUS).token_as(token.kind);
DONE 

TEST(test_normal_plus_with_simple_statement)
    const char *source = "a + b = c";

    Lexer lexer(source);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();
    auto t8 = lexer.next_token();
    auto t9 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(t1, 1, 2, 0);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t1, "a", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(t2, 1, 3, 1);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t2, " ", 1);

    VERIFY_THIS(TokenKind::PLUS).token_as(t3.kind);
    VERIFY_THIS(TokenKind::PLUS).token_metadata_eq_to(t3, 1, 4, 2);
    VERIFY_THIS(TokenKind::PLUS).token_str_eq_to(t3, "+", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(t4, 1, 5, 3);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t4, " ", 1);

    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(t5, 1, 6, 4);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t5, "b", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(t6, 1, 7, 5);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t6, " ", 1);

    VERIFY_THIS(TokenKind::EQUAL).token_as(t7.kind);
    VERIFY_THIS(TokenKind::EQUAL).token_metadata_eq_to(t7, 1, 8, 6);
    VERIFY_THIS(TokenKind::EQUAL).token_str_eq_to(t7, "=", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t8.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(t8, 1, 9, 7);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t8, " ", 1);

    VERIFY_THIS(TokenKind::IDENT).token_as(t9.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(t9, 1, 10, 8);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t9, "c", 1);
DONE 

TEST_ENTRY
    RUN_TEST(test_normal_plus_operator)
    RUN_TEST(test_normal_plus_plus_operator)
    RUN_TEST(test_normal_plus_with_simple_statement);
ENTRY_END
