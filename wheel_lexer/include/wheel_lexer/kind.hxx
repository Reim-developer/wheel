#if !defined(KIND_HXX)
#define KIND_HXX

#include <cstdint>
#include "aliases.hxx"
#include "__wheel_lexer_config.hxx"

WHEEL_LEXER_NAMESPACE 
    enum class TokenKind : uint8_t {
        EOF_,
        ERROR,

        TAB,
        NEWLINE,
        SPACE,

        /*
        C-style comments.
        */
        COMMENT,
        /*
        Document comment.
        */
        DOCUMENT_COMMENT,

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
        /* `++` */
        PLUS_PLUS,
        /* `--` */
        MINUS_MINUS,
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
        LEFT_PARENT,
        /* `(` */
        RIGHT_PARENT
    };

    WHEEL_COMPTIME_NODISCARD_INLINE const char* to_string(TokenKind token_kind) noexcept {
        switch (token_kind) {
            case TokenKind::EOF_:               return "EOF";
            case TokenKind::ERROR:              return "ERROR";
            case TokenKind::TAB:                return "TAB";
            case TokenKind::SPACE:              return "SPACE";
            case TokenKind::COMMENT:            return "COMMENT";
            case TokenKind::DOCUMENT_COMMENT:   return "DOCUMENT_COMMENT";
            case TokenKind::NEWLINE:            return "NEWLINE";
            case TokenKind::IDENT:              return "IDENT";
            case TokenKind::INT_LITERAL:        return "INT_LITERAL";
            case TokenKind::FLOAT_LITERAL:      return "FLOAT_LITERAL";
            case TokenKind::STRING_LITERAL:     return "STRING_LITERAL";
            case TokenKind::EQUAL:              return "EQUAL";
            case TokenKind::PLUS:               return "PLUS";
            case TokenKind::PLUS_PLUS:          return "PLUS_PLUS";
            case TokenKind::MINUS_MINUS:        return "MINUS_MINUS";
            case TokenKind::MINUS:              return "MINUS";
            case TokenKind::STAR:               return "STAR";
            case TokenKind::SLASH:              return "SLASH";
            case TokenKind::EQUAL_EQUAL:        return "EQUAL_EQUAL";
            case TokenKind::NOT_EQUAL:          return "NOT_EQUAL";
            case TokenKind::LESS_THAN:          return "LESS_THAN";
            case TokenKind::GREATER_THAN:       return "GREATER_THAN";
            case TokenKind::COLON:              return "COLON";
            case TokenKind::COMMA:              return "COMMA";
            case TokenKind::LEFT_PARENT:         return "LEFT_PARENT";
            case TokenKind::RIGHT_PARENT:       return  "RIGHT_PARENT";
        }

        return "ERROR";
    }

END_NAMESPACE

#endif // KIND_HXX