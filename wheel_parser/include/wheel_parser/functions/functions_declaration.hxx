#if !defined (FUNCTIONS_DECLARATION_HXX)
#define FUNCTIONS_DECLARATION_HXX
#include "contracts_declaration.hxx"
#include "expression_declaration.hxx"
#include "wheel_parser/parser_utils.hxx"
#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/ast/symbol.hxx"
#include "wheel_parser/functions/emit_error.hxx"
#include "wheel_parser/ast/keywords.hxx"

#include <wheel_lexer/kind.hxx>
#include <wheel_utils/logging.hxx>
#include <format>

using wheel_parser::ast::SymbolID;
#if defined (WHEEL_EXPERIMENT) && defined (WHEEL_SMALL_VEC)
    #include <wheel_memory/vec.hxx>
    using wheel_memory::SmallVec;
    using ArgList = SmallVec<SymbolID>;
#else
    #include <vector>
    using ArgList = std::vector<SymbolID>;
#endif

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    using wheel_parser::ast::StatementNode;
    using wheel_parser::ast::SymbolID;
    using wheel_parser::ast::Keyword;
    using wheel_parser::ast::FunctionDeclaration;
    using wheel_parser::functions::ParserFuncContract;
    using wheel_parser::ParseErrorCode;
    using wheel_lexer::TokenKind;

    inline StatementNode *parse_function_params(ParserFuncContract contract, ArgList &params) {
        if (!token_matches(contract.state.current_token.kind, TokenKind::LEFT_PARENT)) {
            return emit_error(ERROR_CONTEXT(
                ParseErrorCode::ExpectedLeftParent,
                std::format("expected '(' after function name, but got '{}'",
                    contract.state.current_token.str)
            ));
        }
        consume(contract.lexer, contract.state.current_token);

        while (true) {
            if (token_matches(contract.state.current_token.kind, TokenKind::RIGHT_PARENT)) {
                consume(contract.lexer, contract.state.current_token);
                break;
            }

            const auto arg_name = contract.state.current_token;

            if (!token_matches(arg_name.kind, TokenKind::IDENT)) {
                return emit_error(ERROR_CONTEXT(
                    ParseErrorCode::ExpectedIdentifier,
                    std::format("expected parameter name, but got '{}'",
                        arg_name.str)
                ));
            }

            if (matches_any_keywords(arg_name)) {
                return emit_error(ERROR_CONTEXT(
                    ParseErrorCode::ExpectedFunctionName,
                    std::format(
                        "parameter name '{}' conflicts with keyword.\n"
                        "[HINT]: Rename to '_{}' or use another name",
                        arg_name.str, arg_name.str
                    )
                ));
            }

            consume(contract.lexer, contract.state.current_token);

            if (!token_matches(contract.state.current_token.kind, TokenKind::COLON)) {
                return emit_error(ERROR_CONTEXT(
                    ParseErrorCode::ExpectedColon,
                    std::format(
                        "expected ':' after parameter '{}', but got '{}'.\n"
                        "[HINT]: Type annotation should be: {}: int",
                        arg_name.str,
                        contract.state.current_token.str,
                        arg_name.str
                    )
                ));
            }
            consume(contract.lexer, contract.state.current_token);

            // Parse type name
            if (!token_matches(contract.state.current_token.kind, TokenKind::IDENT)) {
                return emit_error(ERROR_CONTEXT(
                    ParseErrorCode::ExpectedType,
                    std::format("expected type name, but got '{}'",
                        contract.state.current_token.str)
                ));
            }
            consume(contract.lexer, contract.state.current_token);

            // Check for comma or end
            if (token_matches(contract.state.current_token.kind, TokenKind::COMMA)) {
                consume(contract.lexer, contract.state.current_token);
                // Continue to next parameter
            }
            // No comma → loop will check for ')' on next iteration
        }

        return nullptr;
    }

    PARSE_BEGIN(parse_function_declaration, StatementNode)
        auto arena = &contract.arena;
        auto interner = &contract.interner;

        if (!keyword_matches(contract.state.current_token, Keyword::Function)) {
            return nullptr;
        }

        const auto func_keyword_token = copy_token(*arena, contract.state.current_token);
        consume(contract.lexer, contract.state.current_token);

        if (!token_matches(contract.state.current_token.kind, TokenKind::IDENT)) {
            return emit_error(ERROR_CONTEXT(
                ParseErrorCode::ExpectedIdentifier,
                std::format("expected function name, but got '{}'",
                    contract.state.current_token.str)
            ));
        }

        if (matches_any_keywords(contract.state.current_token)) {
            return emit_error(ERROR_CONTEXT(
                ParseErrorCode::ExpectedFunctionName,
                std::format(
                    "function name '{}' conflicts with keyword.\n"
                    "[HINT]: Rename to '_{}' or use another name",
                    contract.state.current_token.str,
                    contract.state.current_token.str
                )
            ));
        }

        const auto func_name_token = copy_token(*arena, contract.state.current_token);

        consume(contract.lexer, contract.state.current_token);

        ArgList params(arena);
        auto params_err = parse_function_params(contract, params);
        if (params_err) return params_err;

        SymbolID return_type;
        if (token_matches(contract.state.current_token.kind, TokenKind::ARROW)) {
            consume(contract.lexer, contract.state.current_token);

            if (!token_matches(contract.state.current_token.kind, TokenKind::IDENT)) {
                return emit_error(ERROR_CONTEXT(
                    ParseErrorCode::ExpectedType,
                    std::format("expected return type, but got '{}'",
                        contract.state.current_token.str)
                ));
            }

            consume(contract.lexer, contract.state.current_token);
        }

        if (!token_matches(contract.state.current_token.kind, TokenKind::COLON)) {
            return emit_error(ERROR_CONTEXT(
                ParseErrorCode::ExpectedColon,
                std::format("expected ':' before function body, but got '{}'",
                    contract.state.current_token.str)
            ));
        }
        consume(contract.lexer, contract.state.current_token);

        return nullptr;
    PARSE_END

WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif // FUNCTIONS_DECLARATION_HXX