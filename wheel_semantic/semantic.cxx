#include "wheel_semantic/semantic.hxx"
#include "wheel_semantic/literal_semantic.hxx"

#if defined (WHEEL_DEBUG)
    #include "wheel_utils/logging.hxx"
#endif

#include <utility>
#include <wheel_lexer/kind.hxx>

using wheel_parser::ast::BuiltinFunction;
using wheel_parser::ast::CallExpression;
using wheel_parser::ast::ExpressionNode;
using wheel_parser::ast::ExpressionStatement;
using wheel_parser::ast::IdentifierExpression;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::intern_builtin_registry;
using wheel_semantic::DeclaredSymbol;
using wheel_semantic::DefineGlobalStatement;
using wheel_semantic::Operand;
using wheel_semantic::OperandList;
using wheel_semantic::PrintlnStatement;
using wheel_semantic::SemanticAnalyzer;
using wheel_semantic::SemanticError;
using wheel_semantic::SemanticErrorCode;
using wheel_semantic::ValueKind;
using wheel_lexer::Token;
using wheel_lexer::TokenKind;

[[nodiscard]] const char *semantic_error_message(SemanticErrorCode code) noexcept {
    switch (code) {
        case SemanticErrorCode::DUPLICATE_SYMBOL:
            return "duplicate variable declaration";
        case SemanticErrorCode::UNSUPPORTED_TYPE:
            return "unsupported type annotation";
        case SemanticErrorCode::TYPE_MISMATCH:
            return "initializer literal does not match declared type";
        case SemanticErrorCode::INVALID_STRING_LITERAL:
            return "invalid string literal value";
        case SemanticErrorCode::UnsupportedNode:
            return "unsupported statement node for semantic analysis";
        case SemanticErrorCode::InvalidInitializer:
            return "variable initializer must be a literal expression";
        case SemanticErrorCode::UnsupportedFunction:
            return "unsupported function call";
        case SemanticErrorCode::UndefinedSymbol:
            return "undefined symbol";
        case SemanticErrorCode::InvalidFormatString:
            return "invalid println format string";
        case SemanticErrorCode::InvalidArgumentCount:
            return "invalid argument count for builtin function";
        case SemanticErrorCode::FormatArgumentMismatch:
            return "format placeholder count does not match arguments";
        case SemanticErrorCode::UnsupportedExpression:
            return "unsupported expression for semantic analysis";
        case SemanticErrorCode::INVALID_INTEGER_LITERAL:
            return "invalid integer value";
        case SemanticErrorCode::INTEGER_OUT_OF_RANGE:
            return "integer out of range";
    }

    return "unknown semantic error";
}


#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
SemanticAnalyzer::SemanticAnalyzer(Arena &arena, StringInterner &interner) noexcept :
    m_arena(arena),
    m_builtins(intern_builtin_registry(interner)),
    m_errors(&arena),
    m_program(&arena),
    m_symbols(&arena) {
}
#else
SemanticAnalyzer::SemanticAnalyzer(Arena &arena, StringInterner &interner) noexcept :
    m_arena(arena),
    m_builtins(intern_builtin_registry(interner)),
    m_errors(),
    m_program(),
    m_symbols() {
}
#endif

void SemanticAnalyzer::push_error(SemanticErrorCode code, const Token *token) noexcept {
    m_errors.push_back(SemanticError{
        code,
        token,
        semantic_error_message(code)
    });
}

bool SemanticAnalyzer::parse_string_literal(
    const Token *token,
    std::string_view &value
) const noexcept {
    if (token == nullptr) {
        return false;
    }

    const std::string_view text = token->str;
    switch (token->kind) {
        case TokenKind::STRING_LITERAL:
            if (text.size() < 2 || text.front() != '"' || text.back() != '"') {
                return false;
            }

            value = text.substr(1, text.size() - 2);
            return true;
        case TokenKind::RAW_STRING_LITERAL:
            if (text.size() < 3 || text[0] != 'r' || text[1] != '"' || text.back() != '"') {
                return false;
            }

            value = text.substr(2, text.size() - 3);
            return true;
        default:
            return false;
    }
}

bool SemanticAnalyzer::count_format_placeholders(
    std::string_view format_string,
    size_t &placeholder_count
) const noexcept {
    placeholder_count = 0;

    for (size_t index = 0; index < format_string.size(); ++index) {
        const char character = format_string[index];
        if (character == '{') {
            if (index + 1 >= format_string.size() || format_string[index + 1] != '}') {
                return false;
            }

            ++placeholder_count;
            ++index;
            continue;
        }

        if (character == '}') {
            return false;
        }
    }

    return true;
}

const DeclaredSymbol *SemanticAnalyzer::find_symbol(SymbolID name) const noexcept {
    const DeclaredSymbol *symbols = m_symbols.data();

    for (size_t index = 0; index < m_symbols.size(); ++index) {
        if (symbols[index].name == name) {
            #if defined (WHEEL_DEBUG)
                
                DEBUG_PRINT(FORMAT("Found Symbol: {}", symbols[index].token->str));
            #endif 
            return &symbols[index];
        }
    }

    return nullptr;
}

bool SemanticAnalyzer::resolve_type(SymbolID type_symbol, ValueKind &type_kind) const noexcept {
    wheel_parser::ast::BuiltinType builtin_type = wheel_parser::ast::BuiltinType::Int;
    if (!m_builtins.resolve_type(type_symbol, builtin_type)) {
        return false;
    }

    switch (builtin_type) {
        case wheel_parser::ast::BuiltinType::Int:
            type_kind = ValueKind::Int;
            return true;
        case wheel_parser::ast::BuiltinType::String:
            type_kind = ValueKind::String;
            return true;
        case wheel_parser::ast::BuiltinType::Count:
            break;
    }

    return false;
}

bool SemanticAnalyzer::build_operand(
    const ExpressionNode *expression,
    Operand &operand,
    ValueKind &value_kind
) noexcept {
    if (expression == nullptr || expression->token == nullptr) {
        push_error(SemanticErrorCode::UnsupportedExpression, nullptr);
        return false;
    }

    switch (expression->node_kind) {
        case NodeKind::LiteralExpression: {
            const Token *literal_token = expression->token;
            switch (literal_token->kind) {
                case TokenKind::INT_LITERAL: {
                    int64_t parsed_value = 0;
                    const auto senematic_result = semantic_integer_literal(literal_token->str, parsed_value);

                    if (senematic_result.has_value()) {
                        push_error(senematic_result.value(), literal_token);
                        return false;
                    }

                    value_kind = ValueKind::Int;
                    operand = Operand::from_constant(
                        literal_token,
                        Value::from_int(parsed_value)
                    );
                    return true;
                }
                case TokenKind::STRING_LITERAL:
                case TokenKind::RAW_STRING_LITERAL: {
                    std::string_view parsed_string;
                    if (!parse_string_literal(literal_token, parsed_string)) {
                        push_error(SemanticErrorCode::INVALID_STRING_LITERAL, literal_token);
                        return false;
                    }

                    value_kind = ValueKind::String;
                    operand = Operand::from_constant(
                        literal_token,
                        Value::from_string(parsed_string)
                    );
                    return true;
                }
                default:
                    push_error(SemanticErrorCode::UnsupportedNode, literal_token);
                    return false;
            }
        }
        case NodeKind::IdentifierExpression: {
            const auto *identifier = static_cast<const IdentifierExpression *>(expression);
            const DeclaredSymbol *symbol = find_symbol(identifier->identifier_id);
            if (symbol == nullptr) {
                push_error(SemanticErrorCode::UndefinedSymbol, expression->token);
                return false;
            }

            value_kind = symbol->type;
            operand = Operand::from_binding(expression->token, identifier->identifier_id);
            return true;
        }
        default:
            push_error(SemanticErrorCode::UnsupportedExpression, expression->token);
            return false;
    }
}

bool SemanticAnalyzer::analyze_variable_declaration(
    const VariableDeclaration *declaration
) noexcept {
    if (declaration == nullptr) {
        push_error(SemanticErrorCode::UnsupportedNode, nullptr);
        return false;
    }

    if (find_symbol(declaration->var_name) != nullptr) {
        push_error(SemanticErrorCode::DUPLICATE_SYMBOL, declaration->token);
        return false;
    }

    if (declaration->initializer == nullptr || declaration->initializer->token == nullptr) {
        push_error(SemanticErrorCode::InvalidInitializer, declaration->token);
        return false;
    }

    if (declaration->initializer->node_kind != NodeKind::LiteralExpression) {
        push_error(SemanticErrorCode::InvalidInitializer, declaration->initializer->token);
        return false;
    }

    ValueKind declared_type;
    if (!resolve_type(declaration->var_type, declared_type)) {
        push_error(SemanticErrorCode::UNSUPPORTED_TYPE, declaration->token);
        return false;
    }

    Operand initializer = Operand::from_constant(nullptr, Value::from_int(0));
    ValueKind initializer_type;
    if (!build_operand(declaration->initializer, initializer, initializer_type)) {
        return false;
    }

    if (declared_type != initializer_type) {
        push_error(SemanticErrorCode::TYPE_MISMATCH, declaration->initializer->token);
        return false;
    }

    m_symbols.push_back(DeclaredSymbol{
        declaration->var_name,
        declared_type,
        declaration->token
    });

    m_program.push_back(
        m_arena.allocate<DefineGlobalStatement>(
            declaration->token,
            declaration->var_name,
            initializer
        )
    );

    return true;
}

bool SemanticAnalyzer::analyze_call_expression(
    const CallExpression *expression,
    const Token *statement_token
) noexcept {
    if (expression == nullptr) {
        push_error(SemanticErrorCode::UnsupportedExpression, statement_token);
        return false;
    }

    BuiltinFunction builtin_function = BuiltinFunction::Println;
    if (!m_builtins.resolve_function(expression->callee, builtin_function)) {
        push_error(SemanticErrorCode::UnsupportedFunction, expression->token);
        return false;
    }

    switch (builtin_function) {
        case BuiltinFunction::Println:
            break;
        case wheel_parser::ast::BuiltinFunction::Count:
            push_error(SemanticErrorCode::UnsupportedFunction, expression->token);
            return false;
    }

    const size_t argument_count = expression->arguments.size();
    if (argument_count < 1) {
        push_error(SemanticErrorCode::InvalidArgumentCount, expression->token);
        return false;
    }

    const ExpressionNode *format_expression = expression->arguments[0];
    if (format_expression == nullptr || format_expression->token == nullptr ||
        format_expression->node_kind != NodeKind::LiteralExpression) {
        push_error(SemanticErrorCode::InvalidFormatString, statement_token);
        return false;
    }

    std::string_view format_string;
    if (!parse_string_literal(format_expression->token, format_string)) {
        push_error(SemanticErrorCode::InvalidFormatString, format_expression->token);
        return false;
    }

    size_t placeholder_count = 0;
    if (!count_format_placeholders(format_string, placeholder_count)) {
        push_error(SemanticErrorCode::InvalidFormatString, format_expression->token);
        return false;
    }

    const size_t runtime_argument_count = argument_count - 1;
    if (placeholder_count != runtime_argument_count) {
        push_error(SemanticErrorCode::FormatArgumentMismatch, format_expression->token);
        return false;
    }

    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        OperandList runtime_arguments(&m_arena);
    #else
        OperandList runtime_arguments;
        runtime_arguments.reserve(runtime_argument_count);
    #endif

    for (size_t index = 1; index < argument_count; ++index) {
        Operand runtime_argument = Operand::from_constant(nullptr, Value::from_int(0));
        ValueKind argument_type = ValueKind::Int;
        if (!build_operand(expression->arguments[index], runtime_argument, argument_type)) {
            return false;
        }

        runtime_arguments.push_back(runtime_argument);
    }

    m_program.push_back(
        m_arena.allocate<PrintlnStatement>(
            statement_token,
            format_string,
            std::move(runtime_arguments)
        )
    );

    return true;
}

bool SemanticAnalyzer::analyze_expression_statement(
    const ExpressionStatement *statement
) noexcept {
    if (statement == nullptr || statement->expression == nullptr) {
        push_error(SemanticErrorCode::UnsupportedNode, statement != nullptr ? statement->token : nullptr);
        return false;
    }

    switch (statement->expression->node_kind) {
        case NodeKind::CallExpression:
            return analyze_call_expression(
                static_cast<const CallExpression *>(statement->expression),
                statement->token
            );
        default:
            push_error(SemanticErrorCode::UnsupportedExpression, statement->expression->token);
            return false;
    }
}

bool SemanticAnalyzer::analyze_statement(const StatementNode *statement) noexcept {
    if (statement == nullptr) {
        push_error(SemanticErrorCode::UnsupportedNode, nullptr);
        return false;
    }

    switch (statement->node_kind) {
        case NodeKind::VariableDeclaration:
            return analyze_variable_declaration(
                static_cast<const VariableDeclaration *>(statement)
            );
        case NodeKind::ExpressionStatement:
            return analyze_expression_statement(
                static_cast<const ExpressionStatement *>(statement)
            );
        default:
            push_error(SemanticErrorCode::UnsupportedNode, statement->token);
            return false;
    }
}

bool SemanticAnalyzer::analyze(StatementNode *const *statements, size_t count) noexcept {
    reset();
    if (statements == nullptr && count > 0) {
        push_error(SemanticErrorCode::UnsupportedNode, nullptr);
        return false;
    }

    bool ok = true;
    for (size_t index = 0; index < count; ++index) {
        if (!analyze_statement(statements[index])) {
            ok = false;
        }
    }

    return ok && m_errors.size() == 0;
}

void SemanticAnalyzer::reset() noexcept {
    m_errors.clear();
    m_program.clear();
    m_symbols.clear();
}

wheel_semantic::ProgramView SemanticAnalyzer::program() const noexcept {
    return wheel_semantic::ProgramView{
        m_program.data(),
        m_program.size()
    };
}

size_t SemanticAnalyzer::statement_count() const noexcept {
    return m_program.size();
}

const wheel_semantic::ExecutableStatement *const *SemanticAnalyzer::statements_data() const noexcept {
    return m_program.data();
}

size_t SemanticAnalyzer::error_count() const noexcept {
    return m_errors.size();
}

const SemanticError *SemanticAnalyzer::errors_data() const noexcept {
    return m_errors.data();
}
