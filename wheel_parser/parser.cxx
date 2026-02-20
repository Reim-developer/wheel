#include "wheel_parser/parser.hxx"
#include "include/wheel_parser/ast/nodes.hxx"
#include <wheel_utils/logging.hxx>

using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::LiteralExpression;
using wheel_lexer::Lexer;

WheelParser::WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept : 
                        m_lexer(lexer), m_arena(arena),
                        m_interner(interner) {

}

const Token& WheelParser::consume() noexcept {
    const Token& consumed = m_current_token;
    next_token();

    return consumed;
}

bool WheelParser::str_matches(string_view str) noexcept {
      return m_current_token.str == str;
}

bool WheelParser::token_matches(TokenKind token_kind) noexcept {
    return m_current_token.kind == token_kind;
}

const Token *WheelParser::copy_token() const noexcept {
    return m_arena.allocate<Token>(
        m_current_token.kind,
        m_current_token.str,
        m_current_token.start,
        m_current_token.end
    );
}

// TODO: Implement error reporting, avoid returning `nullptr`
// We also need to improve error handling and avoid assuming everything is literal.
VariableDeclaration *WheelParser::parse_variable_declaration() noexcept {
    consume();
    const auto start_token = copy_token();

    if (token_matches(TokenKind::IDENT) && str_matches("int")) {
        while (true) {
            consume();
            if (m_current_token.kind != TokenKind::SPACE) break;
        }

        const auto var_type = m_interner.intern("int");
        const auto var_name = m_interner.intern(m_current_token.str);

        while(true) {
            consume();
            if (m_current_token.kind != TokenKind::SPACE) break;
        }

        const auto var_operator = m_current_token;
        if (var_operator.kind != TokenKind::EQUAL) {
            return nullptr;
        }

        while(true) {
            consume();
            if (m_current_token.kind != TokenKind::SPACE) break;
        }

        const auto var_value = m_interner.intern(m_current_token.str);
        const auto literal = m_arena.allocate<LiteralExpression>(
            &m_current_token
        );

        return m_arena.allocate<VariableDeclaration>(
            start_token,
            var_type, var_name, literal
        );
    }

    return nullptr;
}

void WheelParser::parse() noexcept {
}
