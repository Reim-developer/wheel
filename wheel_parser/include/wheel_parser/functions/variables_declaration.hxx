#include "wheel_parser/error.hxx"
#if !defined(PARSER_VARIABLES_DECLARATION_HXX)
#define PARSER_VARIABLES_DECLARATION_HXX

#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/parser_utils.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include "wheel_parser/ast/builtins.hxx"
#include "contracts_declaration.hxx"
#include "emit_error.hxx"

using wheel_parser::ast::StatementNode;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::functions::ParserFuncContract;
using wheel_parser::functions::EmitErrorConfig;
using wheel_parser::ast::Keyword;
using wheel_parser::ast::LiteralExpression;
using wheel_parser::ast::BuiltinRegistry;

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    PARSE_BEGIN(parse_variable_declaration, StatementNode, BuiltinRegistry registry) 
        consume(contract.lexer, contract.state.current_token);
        if (!keyword_matches(contract.state.current_token, Keyword::Var)) {
            return nullptr;
        }

        const auto start_token = copy_token(contract.arena, contract.state.current_token);

        skip_spaces(contract.lexer, contract.state.current_token);

        if (!token_matches(contract.state.current_token.kind, TokenKind::IDENT)) {
            return emit_error(ERROR_CONTEXT(ParseErrorCode::ExpectedIdentifier, ""));
        }
        const auto var_name = contract.interner.intern(contract.state.current_token.str);

        skip_spaces(contract.lexer, contract.state.current_token);
        if (!token_matches(contract.state.current_token.kind, TokenKind::COLON)) {
            return emit_error(ERROR_CONTEXT(ParseErrorCode::ExpectedColon, ""));
        }

        skip_spaces(contract.lexer, contract.state.current_token);
        if (!token_matches(contract.state.current_token.kind, TokenKind::IDENT)) {
            return emit_error(ERROR_CONTEXT(ParseErrorCode::ExpectedTypeKeyword, ""));
        }

        BuiltinType builtin_type;
        if (!type_keyword_matches(contract.state.current_token, builtin_type)) {
            builtin_type = BuiltinType::ManualDeclaration;
        }

        const auto var_type = registry.type_symbol(builtin_type);

        skip_spaces(contract.lexer, contract.state.current_token);
        if (!token_matches(contract.state.current_token.kind, TokenKind::EQUAL)) {
            return emit_error(ERROR_CONTEXT(ParseErrorCode::ExpectedEqual, ""));
        }

        skip_spaces(contract.lexer, contract.state.current_token);
        switch (contract.state.current_token.kind) {
            case TokenKind::INT_LITERAL:
            case TokenKind::FLOAT_LITERAL:
            case TokenKind::STRING_LITERAL:
            case TokenKind::RAW_STRING_LITERAL:
                break;

            default:
                /*  In this case, we'll ignore for semantic phase 
                    for handling this case:

                    var x: int = -1 
                */
                IGNORE;
        }

        const auto literal_token = copy_token(contract.arena, contract.state.current_token);
        const auto literal = contract.arena.allocate<LiteralExpression>(
            literal_token
        );

        return contract.arena.allocate<VariableDeclaration>(
            start_token,
            var_type, var_name, literal
        );

    PARSE_END

WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif // PARSER_VARIABLES_DECLARATION_HXX