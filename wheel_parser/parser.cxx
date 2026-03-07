#include "wheel_parser/parser.hxx"
#include "wheel_parser/ast/builtins.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/parser_utils.hxx"
#include <utility>
#include <wheel_utils/logging.hxx>

using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::LiteralExpression;
using wheel_parser::ast::IdentifierExpression;
using wheel_parser::ast::CallExpression;
using wheel_parser::ast::ExpressionNode;
using wheel_parser::ast::ExpressionStatement;
using wheel_parser::ast::ErrorStatement;
using wheel_parser::ast::BuiltinType;
using wheel_parser::ast::BuiltinFunction;
using wheel_parser::ast::SymbolID;
using wheel_parser::ast::lookup_builtin_function;
using wheel_parser::ast::intern_builtin_registry;
using wheel_parser::ast::Keyword;
using wheel_lexer::Lexer;

#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
WheelParser::WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept :
    m_lexer(lexer),
    m_arena(arena),
    m_interner(interner),
    m_builtins(intern_builtin_registry(interner)),
    m_errors(&arena),
    m_statements(&arena) {}
#else
WheelParser::WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept :
    m_lexer(lexer),
    m_arena(arena),
    m_interner(interner),
    m_builtins(intern_builtin_registry(interner)),
    m_errors(),
    m_statements() {}
#endif

void WheelParser::skip_spaces() noexcept {
    while (true) {
        consume(m_lexer, m_current_token);
        if (m_current_token.kind != TokenKind::SPACE && m_current_token.kind != TokenKind::TAB) {
            break;
        }
    }
}

void WheelParser::synchronize_statement() noexcept {
    while (m_current_token.kind != TokenKind::EOF_ && m_current_token.kind != TokenKind::NEWLINE) {
        consume(m_lexer, m_current_token);
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

ExpressionNode *WheelParser::parse_expression() noexcept {
    switch (m_current_token.kind) {
        case TokenKind::INT_LITERAL:
        case TokenKind::FLOAT_LITERAL:
        case TokenKind::STRING_LITERAL:
        case TokenKind::RAW_STRING_LITERAL:
            return m_arena.allocate<LiteralExpression>(copy_token());
            
        case TokenKind::IDENT:
            return m_arena.allocate<IdentifierExpression>(
                copy_token(),
                m_interner.intern(m_current_token.str)
            );
        default:
            return nullptr;
    }
}

StatementNode *WheelParser::parse_variable_declaration() noexcept {
    consume(m_lexer, m_current_token);
    if (!keyword_matches(m_current_token, Keyword::Var)) {
        return nullptr;
    }

    const auto start_token = copy_token();

    skip_spaces();

    if (!token_matches(m_current_token.kind, TokenKind::IDENT)) {
        return emit_error(ParseErrorCode::ExpectedIdentifier);
    }
    const auto var_name = m_interner.intern(m_current_token.str);

    skip_spaces();
    if (!token_matches(m_current_token.kind, TokenKind::COLON)) {
        return emit_error(ParseErrorCode::ExpectedColon);
    }

    skip_spaces();
    if (!token_matches(m_current_token.kind, TokenKind::IDENT)) {
        return emit_error(ParseErrorCode::ExpectedTypeKeyword);
    }

    BuiltinType builtin_type = BuiltinType::Int;
    if (!type_keyword_matches(m_current_token, builtin_type)) {
        return emit_error(ParseErrorCode::ExpectedTypeKeyword);
    }

    const auto var_type = m_builtins.type_symbol(builtin_type);

    skip_spaces();
    if (!token_matches(m_current_token.kind, TokenKind::EQUAL)) {
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

StatementNode *WheelParser::parse_call_statement() noexcept {
    if (!token_matches(m_current_token.kind, TokenKind::IDENT)) {
        return nullptr;
    }

    const auto start_token = copy_token();
    SymbolID callee = m_interner.intern(m_current_token.str);

    BuiltinFunction builtin_function = BuiltinFunction::Println;
    if (lookup_builtin_function(m_current_token.str, builtin_function)) {
        callee = m_builtins.function_symbol(builtin_function);
    }

    skip_spaces();
    if (!token_matches(m_current_token.kind, TokenKind::LEFT_PARENT)) {
        return emit_error(ParseErrorCode::ExpectedLeftParent);
    }

    skip_spaces();
    std::vector<ExpressionNode *> arguments;

    if (!token_matches(m_current_token.kind, TokenKind::RIGHT_PARENT)) {
        while (true) {
            ExpressionNode *argument = parse_expression();
            if (argument == nullptr) {
                return emit_error(ParseErrorCode::ExpectedExpression);
            }

            arguments.push_back(argument);
            skip_spaces();

            if (token_matches(m_current_token.kind, TokenKind::RIGHT_PARENT)) {
                break;
            }

            if (token_matches(m_current_token.kind, TokenKind::EOF_) ||
                token_matches(m_current_token.kind, TokenKind::NEWLINE)) {
                return emit_error(ParseErrorCode::ExpectedRightParent);
            }

            if (!argument_separator_matches(m_current_token.kind)) {
                return emit_error(ParseErrorCode::ExpectedArgumentSeparator);
            }

            skip_spaces();
            if (token_matches(m_current_token.kind, TokenKind::RIGHT_PARENT)) {
                return emit_error(ParseErrorCode::ExpectedExpression);
            }
        }
    }

    auto *call_expression = m_arena.allocate<CallExpression>(
        start_token,
        callee,
        std::move(arguments)
    );

    return m_arena.allocate<ExpressionStatement>(start_token, call_expression);
}

StatementNode *WheelParser::parse_statement() noexcept {
    if (StatementNode *variable_declaration = parse_variable_declaration();
        variable_declaration != nullptr) {
        return variable_declaration;
    }

    if (StatementNode *call_statement = parse_call_statement();
        call_statement != nullptr) {
        return call_statement;
    }

    return nullptr;
}

void WheelParser::parse() noexcept {
    clear_errors();
    m_statements.clear();

    while (true) {
        StatementNode *statement = parse_statement();

        if (m_current_token.kind == TokenKind::EOF_) {
            break;
        }

        if (statement != nullptr) {
            m_statements.push_back(statement);
        }

        if (statement == nullptr) {
            if (m_current_token.kind == TokenKind::NEWLINE) {
                continue;
            }

            if (m_current_token.kind == TokenKind::SPACE || m_current_token.kind == TokenKind::TAB) {
                continue;
            }

            static_cast<void>(emit_error(ParseErrorCode::UnexpectedStatement));
        }
    }
}

size_t WheelParser::error_count() const noexcept {
    return m_errors.size();
}

const wheel_parser::ParseError *WheelParser::errors_data() const noexcept {
    return m_errors.data();
}

size_t WheelParser::statement_count() const noexcept {
    return m_statements.size();
}

StatementNode *const *WheelParser::statements_data() const noexcept {
    return m_statements.data();
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
