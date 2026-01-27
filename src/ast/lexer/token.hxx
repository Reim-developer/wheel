#if !defined(TOKEN_HXX)
#define TOKEN_HXX

#include <string>
#include <string_view>
#include <__internal_headers/__config.hxx>

using std::string;
using std::string_view;

WHEEL_AST_NAMESPACE :: lexer {
    enum class TokenType {
        /* Keywords */
        INT_KEYWORD,

        /* Literals */
        IDENTIFIER,
        NUMBER,

        /* Operators */
        EQUAL,

        /* Whitespace */
        WHITESPACE,
        NEWLINE,

        /* Special */
        END_OF_LINE
    };

    struct Token {
        TokenType   token_type;
        string_view value;
        size_t      line;
        size_t      column;

        Token(
            TokenType   token_type,
            string_view value,
            size_t      line,
            size_t      column) :
            
            token_type(token_type),
            value(value),
            line(line),
            column(column)
            {}
            
        struct LexerError {};
    };
}

#endif 