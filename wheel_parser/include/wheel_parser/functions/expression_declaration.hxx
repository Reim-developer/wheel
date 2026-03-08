
#if !defined (PARSE_EXPRESSION_DECLARATION_HXX)
#define PARSE_EXPRESSION_DECLARATION_HXX
#include "wheel_parser/config.hxx"
#include "contracts_declaration.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include <wheel_lexer/kind.hxx>
#include "wheel_parser/parser_utils.hxx"

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    using wheel_parser::ast::ExpressionNode;
    using wheel_parser::ast::LiteralExpression;
    using wheel_parser::ast::IdentifierExpression;
    using wheel_lexer::TokenKind;

    PARSE_BEGIN(parse_expression_declaration, ExpressionNode)
        using Kind = TokenKind;
        auto current_token = contract.state.current_token;
        auto arena = &contract.arena;
        auto interner = &contract.interner;

        switch (current_token.kind) {
            case TokenKind::INT_LITERAL:
            case TokenKind::FLOAT_LITERAL:
            case TokenKind::STRING_LITERAL:
            case TokenKind::RAW_STRING_LITERAL:
                return arena->allocate<LiteralExpression>(
                    copy_token(*arena, current_token)
                );

            case TokenKind::IDENT:
                return arena->allocate<IdentifierExpression>(
                    copy_token(*arena, current_token),
                    interner->intern(current_token.str)
                );

            default:
                return nullptr;
        }

    PARSE_END


WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif 