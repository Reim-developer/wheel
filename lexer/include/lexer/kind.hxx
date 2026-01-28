#if !defined(KIND_HXX)
#define KIND_HXX

#include <cstdint>

namespace lexer {
    enum class TokenKind : uint8_t {
        EOF_,
        ERROR,

        TAB,
        NEWLINE,

        IDENT,
        
        /* `10` */
        INT_LITERAL,
        /* `10.5` */
        FLOAT_LITERAL,
        /* `"Hello World"` */
        STRING_LITERAL,
        /* `=` */
        EQUAL,
        /* `+` */
        PLUS,
        /* `-` */
        MINUS,
        /* `*` */
        STAR,
        /* `/` */
        SLASH,
        /* `==` */
        EQUAL_EQUAL,
        /* `!=` */
        NOT_EQUAL,
        /* `<` */
        LESS_THAN,
        /* `>` */
        GREATER_THAN,
        /* `:` */
        COLON,
        /* `,` */
        COMMA,
        /* `)` */ 
        LEFT_PAREN,
        /* `(` */
        RIGHT_PARENT
    };

    [[nodiscard]] const char* to_string(TokenKind token_kind) noexcept {
        switch (token_kind) {
            case TokenKind::EOF_:           return "EOF";
            case TokenKind::ERROR:          return "ERROR";
            case TokenKind::TAB:            return "TAB";
            case TokenKind::NEWLINE:        return "NEWLINE";
            case TokenKind::IDENT:          return "IDENT";
            case TokenKind::INT_LITERAL:    return "INT_LITERAL";
            case TokenKind::FLOAT_LITERAL:  return "FLOAT_LITERAL";
            case TokenKind::STRING_LITERAL: return "STRING_LITERAL";
            case TokenKind::EQUAL:          return "EQUAL";
            case TokenKind::PLUS:           return "PLUS";
            case TokenKind::MINUS:          return "MINUS";
            case TokenKind::STAR:           return "STAR";
            case TokenKind::SLASH:          return "SLASH";
            case TokenKind::EQUAL_EQUAL:    return "EQUAL_EQUAL";
            case TokenKind::NOT_EQUAL:      return "NOT_EQUAL";
            case TokenKind::LESS_THAN:      return "LESS_THAN";
            case TokenKind::GREATER_THAN:   return "GREATER_THAN";
            case TokenKind::COLON:          return "COLON";
            case TokenKind::COMMA:          return "COMMA";
            case TokenKind::LEFT_PAREN:     return "LEFT_PARENT";
            case TokenKind::RIGHT_PARENT:   return  "RIGHT_PARENT";
        }

        return "ERROR";
    }
}

#endif // KIND_HXX