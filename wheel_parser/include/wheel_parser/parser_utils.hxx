#if !defined (PARSER_UTILS_HXX)
#define PARSER_UTILS_HXX 

#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/builtins.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/token.hxx>
#include <wheel_memory/allocator.hxx>

using wheel_lexer::Token;
using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;
using wheel_parser::ast::BuiltinType;
using wheel_parser::ast::Keyword;
using wheel_parser::ast::k_keywords;
using wheel_parser::ast::lookup_builtin_type;
using wheel_memory::Arena;

WHEEL_PARSER_NAMESPACE
    inline void next_token(Lexer &lexer, Token &current_token) noexcept {
        current_token = lexer.next_token();
    }

    inline const Token &consume(Lexer &lexer, Token &current_token) noexcept {
        const Token &consumed = current_token;

        next_token(lexer, current_token);

        return consumed;
    } 

    [[nodiscard]]
    inline bool token_matches(TokenKind current_kind, TokenKind expected_kind) noexcept {
        return current_kind == expected_kind;
    }

    [[nodiscard]]
    inline bool keyword_matches(const Token &token, Keyword keyword) noexcept {
        if (!token_matches(token.kind, TokenKind::IDENT)) {
            return false;
        }

        return token.str == k_keywords[static_cast<size_t>(keyword)].text;
    }

    [[nodiscard]]
    inline bool type_keyword_matches(const Token &token, BuiltinType &builtin_type) noexcept {
        if (!token_matches(token.kind, TokenKind::IDENT)) {
            return false;
        }

        return lookup_builtin_type(token.str, builtin_type);
    }

    inline bool argument_separator_matches(TokenKind token_kind) noexcept {
        return token_kind == TokenKind::COMMA || token_kind == TokenKind::DOT;
    }

    [[nodiscard]]
    inline const Token *copy_token(Arena &arena, const Token &current_token) noexcept {
        return arena.allocate<Token>(
            current_token.kind,
            current_token.str,
            current_token.start,
            current_token.end
        );
    }

    inline void skip_spaces(Lexer &lexer, Token &current_token) noexcept {
        while (current_token.kind != TokenKind::SPACE && current_token.kind != TokenKind::TAB) {
            next_token(lexer, current_token);
        }  
    }

    [[nodiscard]]
    inline constexpr const bool matches_any_keywords(const Token &token) {
        if (!token_matches(token.kind, TokenKind::IDENT)) {
            return false;
        }

        for (size_t index = 0; index < static_cast<size_t>(Keyword::Count); ++index) {
            if (token.str == k_keywords[index].text) {
                return true;
            }
        }

        return false;
    }

    template<typename ... Keywords>
    [[nodiscard]]
    inline constexpr const bool matches_any_keywords(const Token &token, Keywords... keywords) {
        return (keyword_matches(token, keywords) || ... );
    }

WHEEL_PARSER_END_NAMESPACE

#endif // UTILS_HXX
