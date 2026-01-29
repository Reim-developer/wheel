#include "__lexer_test_helper/__helper.hxx"
#include <lexer/lexer.hxx>
#include <lexer/kind.hxx>
#include <__internal/__assert.hxx>
#include <format>

using lexer::Lexer;
using lexer::TokenKind;
using lexer::to_string;
using std::format;

void test_eof_success_with_empty_input() {
    DEBUG_OUTPUT(format("'test_eof_success_with_empty_input'"));
    const char* source = "";

    Lexer lexer(source);
    auto token = lexer.next_token();

    Verify<lexer::TokenKind::EOF_>{}.token_as(lexer::TokenKind::EOF_);
    Verify<lexer::TokenKind::EOF_>{}.token_str_eq_to(token, "", 0);
    Verify<lexer::TokenKind::EOF_>{}.token_metadata_eq_to(token, 1, 1, 0);
}

int main() {
    test_eof_success_with_empty_input();
    return 0;
}