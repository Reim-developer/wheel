#if !defined(UTILS_HXX)
#define UTILS_HXX

#include <__internal_headers/__config.hxx>
#include <unordered_map>
#include <cctype>
#include "token.hxx"

using std::string_view;
using std::unordered_map;
using std::isalpha;
using std::isalnum;
using std::isdigit;
using std::isspace;
using wheel::ast::lexer::TokenType;

WHEEL_AST_NAMESPACE :: lexer {
    inline const unordered_map<string_view, TokenType> keywords = {
        {"int", TokenType::INT_KEYWORD}
    };

    inline bool is_keyword(string_view str) noexcept {
        return keywords.count(str) > 0;
    }

    inline TokenType keyword_type(string_view str) noexcept {
        auto it = keywords.find(str);
        return it != keywords.end() ? 
            it->second : TokenType::IDENTIFIER;
    }

    inline bool is_identifier_start(char character) {
        unsigned char uc = static_cast<unsigned char>(character);
        return isalpha(uc) || uc == '_';
    }

    inline bool is_identifier_continue(char character) {
        unsigned char uc = static_cast<unsigned char>(character);
        return isalnum(uc) || uc == '_';
    }

    inline bool is_digit(char character) {
        unsigned char uc = static_cast<unsigned char>(character);
        return is_digit(uc);
    }

    inline bool is_whitespace(char character) {
        unsigned char uc = static_cast<unsigned char>(character);
        return isspace(uc);
    }

}

#endif // UTILS_HXX