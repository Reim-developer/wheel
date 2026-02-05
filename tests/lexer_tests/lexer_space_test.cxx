#include "__lexer_test_helper/__helper.hxx"
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;

TEST(test_normal_space)
    const char* source = " ";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(token), 1, 1, 0);
    VERIFY_THIS(TokenKind::SPACE).token_as(token.kind);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(token, " ", STR_LEN(source));
DONE

TEST(test_normal_space_with_variable_declaration)
    const char* source = "int x = 40";

    Lexer lexer(source);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t1, "int", 3);
    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);

    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t2), 1, 4, 3);
    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t2, " ", 1);

    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(lexer.get_source_location(t3), 1, 5, 4);
    VERIFY_THIS(TokenKind::IDENT).token_as(t3.kind);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t3, "x", 1);

    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t4), 1, 6, 5);
    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t4, " ", 1);

    VERIFY_THIS(TokenKind::EQUAL).token_metadata_eq_to(lexer.get_source_location(t5), 1, 7, 6);
    VERIFY_THIS(TokenKind::EQUAL).token_as(t5.kind);
    VERIFY_THIS(TokenKind::EQUAL).token_str_eq_to(t5, "=", 1);

    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t6), 1, 8, 7);
    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t6, " ", 1);

    VERIFY_THIS(TokenKind::INT_LITERAL).token_metadata_eq_to(lexer.get_source_location(t7), 1, 9, 8);
    VERIFY_THIS(TokenKind::INT_LITERAL).token_as(t7.kind);
    VERIFY_THIS(TokenKind::INT_LITERAL).token_str_eq_to(t7, "40", 2);
DONE

TEST_ENTRY
    RUN_TEST(test_normal_space)
    RUN_TEST(test_normal_space_with_variable_declaration)
ENTRY_END