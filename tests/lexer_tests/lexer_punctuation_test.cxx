#include "__lexer_test_helper/__helper.hxx"

#include <__internal/__assert.hxx>
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;


TEST(test_punctuation_tokens)
    Lexer lexer("(,.)");

    const auto left_parent = lexer.next_token();
    const auto comma = lexer.next_token();
    const auto dot = lexer.next_token();
    const auto right_parent = lexer.next_token();

    VERIFY_THIS(TokenKind::LEFT_PARENT).token_as(left_parent.kind);
    VERIFY_THIS(TokenKind::LEFT_PARENT).token_str_eq_to(left_parent, "(", 1);

    VERIFY_THIS(TokenKind::COMMA).token_as(comma.kind);
    VERIFY_THIS(TokenKind::COMMA).token_str_eq_to(comma, ",", 1);

    VERIFY_THIS(TokenKind::DOT).token_as(dot.kind);
    VERIFY_THIS(TokenKind::DOT).token_str_eq_to(dot, ".", 1);

    VERIFY_THIS(TokenKind::RIGHT_PARENT).token_as(right_parent.kind);
    VERIFY_THIS(TokenKind::RIGHT_PARENT).token_str_eq_to(right_parent, ")", 1);
DONE

TEST_ENTRY
    RUN_TEST(test_punctuation_tokens)
ENTRY_END
