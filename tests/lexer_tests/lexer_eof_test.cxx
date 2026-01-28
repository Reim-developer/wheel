#include <lexer/lexer.hxx>
#include <lexer/kind.hxx>
#include <__internal/__assert.hxx>
#include <format>

using lexer::Lexer;
using lexer::TokenKind;
using lexer::to_string;
using std::format;

void test_eof_success_with_empty_input() {
    DEBUG_OUTPUT(format("'test_eof_succes_with_empty_input'"));
    const char* source = "";

    Lexer lexer(source);
    auto token          = lexer.next_token();
    auto token_type     = to_string(token.kind);
    auto line_no        = token.source_location.line;
    auto line_col       = token.source_location.column;
    auto line_offset    = token.source_location.offset;

    internal_assert(token.kind == TokenKind::EOF_, format(
        "Expected {}, but got {} of <token.kind>",
        to_string(TokenKind::EOF_), token_type
    ));

    internal_assert(line_no == 1, format(
        "Expected {}, but got {} of <token.source_location.line>",
        1, line_no
    ));
     internal_assert(line_col == 1, format(
        "Expected {}, but got {} of <token.source_location.column>",
        1, line_col
    ));
     internal_assert(line_offset == 0, format(
        "Expected {}, but got {} of <token.source_location.offset>",
        1, line_offset
    ));

    DEBUG_OUTPUT(format(
        "<token_type>: {}", token_type
    ));
    DEBUG_OUTPUT(format(
        "<line_no>: {}", line_no
    ));
    DEBUG_OUTPUT(format(
        "<line_col>: {}", line_col 
    ));
      DEBUG_OUTPUT(format(
        "<line_offset>: {}", line_offset
    ));
}

int main() {
    test_eof_success_with_empty_input();
    return 0;
}