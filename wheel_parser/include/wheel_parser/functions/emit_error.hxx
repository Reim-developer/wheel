#if !defined(PARSER_EMIT_ERROR_HXX)
#define PARSER_EMIT_ERROR_HXX

#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/error.hxx"
#include "wheel_parser/declaration.hxx"
#include "wheel_parser/parser_utils.hxx"
#include <wheel_lexer/token.hxx>
#include <wheel_lexer/kind.hxx>
#include <wheel_lexer/source_map.hxx>
#include <wheel_memory/allocator.hxx>

using wheel_parser::ast::StatementNode;
using wheel_parser::ast::ErrorStatement;
using wheel_parser::ParseErrorCode;
using wheel_lexer::Token;
using wheel_lexer::TokenKind;
using wheel_lexer::SourceLocation;
using wheel_memory::Arena;

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    struct EmitErrorConfig {
        ParseErrorCode code;
        ParserErrorList &error_list;
        const Token *token;
        TokenKind actual;
        SourceLocation source_location;
        Arena &arena;
        Token &current_token;
        Lexer &lexer;
    };

    inline StatementNode *emit_error(EmitErrorConfig config) noexcept {
        const auto *token = copy_token(config.arena, config.current_token);
        const auto error = make_parse_error(
            config.code,
            token,
            token->kind,
            config.lexer.get_source_location(*token)
        );
        config.error_list.push_back(error);

        return config.arena.allocate<ErrorStatement>(token);
    }


WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif 