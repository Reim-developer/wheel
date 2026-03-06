#include "wheel_parser/parser.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include <utility>
#include <wheel_utils/logging.hxx>

using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::ErrorStatement;
using wheel_parser::ast::LiteralExpression;
using wheel_lexer::Lexer;
using wheel_parser::ast::k_keywords;

#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
WheelParser::WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept :
    m_lexer(lexer), m_arena(arena), m_interner(interner), m_errors(&arena) {}
#else
WheelParser::WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept :
    m_lexer(lexer), m_arena(arena), m_interner(interner), m_errors() {}
#endif

const Token& WheelParser::consume() noexcept {
    const Token& consumed = m_current_token;
    next_token();

    return consumed;
}

bool WheelParser::token_matches(TokenKind token_kind) const noexcept {
    return m_current_token.kind == token_kind;
}

bool WheelParser::keyword_matches(Keyword keyword) const noexcept {
    if (m_current_token.kind != TokenKind::IDENT) {
        return false;
    }

    return m_current_token.str == k_keywords[static_cast<size_t>(keyword)].text;
}

void WheelParser::skip_spaces() noexcept {
    while (true) {
        consume();
        if (m_current_token.kind != TokenKind::SPACE && m_current_token.kind != TokenKind::TAB) {
            break;
        }
    }
}

void WheelParser::synchronize_statement() noexcept {
    while (m_current_token.kind != TokenKind::EOF_ && m_current_token.kind != TokenKind::NEWLINE) {
        consume();
    }
}

StatementNode *WheelParser::emit_error(
    ParseErrorCode code
) noexcept {
    const auto *token = copy_token();
    const auto error = make_parse_error(
        code,
        token,
        token->kind,
        m_lexer.get_source_location(*token)
    );
    m_errors.push_back(error);

    synchronize_statement();
    return m_arena.allocate<ErrorStatement>(token);
}

const Token *WheelParser::copy_token() const noexcept {
    return m_arena.allocate<Token>(
        m_current_token.kind,
        m_current_token.str,
        m_current_token.start,
        m_current_token.end
    );
}

StatementNode *WheelParser::parse_variable_declaration() noexcept {
    consume();
    if (!keyword_matches(Keyword::Var)) {
        return nullptr;
    }

    const auto start_token = copy_token();

    skip_spaces();

    if (!token_matches(TokenKind::IDENT)) {
        return emit_error(ParseErrorCode::ExpectedIdentifier);
    }
    const auto var_name = m_interner.intern(m_current_token.str);

    skip_spaces();
    if (!token_matches(TokenKind::COLON)) {
        return emit_error(ParseErrorCode::ExpectedColon);
    }

    skip_spaces();
    if (!token_matches(TokenKind::IDENT)) {
        return emit_error(ParseErrorCode::ExpectedType);
    }

    const auto var_type = m_interner.intern(m_current_token.str);

    skip_spaces();
    if (!token_matches(TokenKind::EQUAL)) {
        return emit_error(ParseErrorCode::ExpectedEqual);
    }

    skip_spaces();
    switch (m_current_token.kind) {
        case TokenKind::INT_LITERAL:
        case TokenKind::FLOAT_LITERAL:
        case TokenKind::STRING_LITERAL:
        case TokenKind::RAW_STRING_LITERAL:
            break;
        default:
            return emit_error(ParseErrorCode::ExpectedLiteral);
    }

    const auto literal_token = copy_token();
    const auto literal = m_arena.allocate<LiteralExpression>(
        literal_token
    );

    return m_arena.allocate<VariableDeclaration>(
        start_token,
        var_type, var_name, literal
    );
}

void WheelParser::parse() noexcept {
}

size_t WheelParser::error_count() const noexcept {
    return m_errors.size();
}

const wheel_parser::ParseError *WheelParser::errors_data() const noexcept {
    return m_errors.data();
}

void WheelParser::clear_errors() noexcept {
    m_errors.clear();
}

wheel_parser::ParseDiagnosticList WheelParser::collect_diagnostics(std::string_view file_name) const {
    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        ParseDiagnosticList diagnostics(&m_arena);
    #else
        ParseDiagnosticList diagnostics;
        diagnostics.reserve(m_errors.size());
    #endif

    const auto *errors = m_errors.data();

    for (size_t index = 0; index < m_errors.size(); ++index) {
        const auto line_view = m_lexer.get_source_line(errors[index].location.offset);
        diagnostics.push_back(build_parse_diagnostic(errors[index], line_view, file_name));
    }

    return ParseDiagnosticList(std::move(diagnostics));
}
