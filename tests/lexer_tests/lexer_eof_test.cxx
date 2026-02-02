#include "__lexer_test_helper/__helper.hxx"
#include <lib/lexer.hxx>
#include <lib/kind.hxx>
#include <__internal/__assert.hxx>
#include <format>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;
using wheel_lexer::to_string;
using std::format;

TEST(test_eof_success_with_empty_input)
    const char* source = "";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::EOF_).token_as(TokenKind::EOF_);
    VERIFY_THIS(TokenKind::EOF_).token_str_eq_to(token, "", 0);
    VERIFY_THIS(TokenKind::EOF_).token_metadata_eq_to(token, 1, 1, 0);
DONE

TEST_ENTRY
    RUN_TEST(test_eof_success_with_empty_input)
ENTRY_END