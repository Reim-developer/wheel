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

TEST_ENTRY
    RUN_TEST(test_normal_plus_operator)
    RUN_TEST(test_normal_plus_plus_operator)
ENTRY_END
