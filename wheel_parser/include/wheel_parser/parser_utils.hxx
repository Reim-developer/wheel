#if !defined (PARSER_UTILS_HXX)
#define PARSER_UTILS_HXX 

#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/builtins.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/token.hxx>

using wheel_lexer::Token;
using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;
using wheel_parser::ast::BuiltinType;
using wheel_parser::ast::Keyword;
using wheel_parser::ast::k_keywords;
using wheel_parser::ast::lookup_builtin_type;

WHEEL_PARSER_NAMESPACE
    inline void next_token(Lexer &lexer, Token &current_token) noexcept {
        current_token = lexer.next_token();
    }

    inline const Token &consume(Lexer &lexer, Token &current_token) noexcept {
        const Token &consumed = current_token;

        next_token(lexer, current_token);

        return consumed;
    } 

    inline bool token_matches(TokenKind current_kind, TokenKind expected_kind) noexcept {
        return current_kind == expected_kind;
    }

    inline bool keyword_matches(const Token &token, Keyword keyword) noexcept {
        if (!token_matches(token.kind, TokenKind::IDENT)) {
            return false;
        }

        return token.str == k_keywords[static_cast<size_t>(keyword)].text;
    }

    inline bool type_keyword_matches(const Token &token, BuiltinType &builtin_type) noexcept {
        if (!token_matches(token.kind, TokenKind::IDENT)) {
            return false;
        }

        return lookup_builtin_type(token.str, builtin_type);
    }

    inline bool argument_separator_matches(TokenKind token_kind) noexcept {
        return token_kind == TokenKind::COMMA || token_kind == TokenKind::DOT;
    }

WHEEL_PARSER_END_NAMESPACE

#endif // UTILS_HXX
