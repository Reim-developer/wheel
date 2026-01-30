#if !defined(HANDLERS_HXX)
#define HANDLERS_HXX
#include "cursor.hxx"
#include "token.hxx"
#include "utils.hxx"
#include "aliases.hxx"
#include "properties.hxx"
#include <string_view>
#include <array>

namespace lexer {
    using TokenHandler = Token(*)(Cursor&, size_t);
    using StrView      = std::string_view;
    using Kind         = TokenKind;

    #define _SOURCE_TEXT(cursor, start) cursor.source_view.substr(start, cursor.position() - start) 

    [[nodiscard]] Token if_tab(Cursor& cursor, size_t start) noexcept {
        cursor.bump();
        return make_token(Kind::TAB, "\t", start, cursor.position());
    }

    [[nodiscard]] Token if_newline(Cursor& cursor, size_t start) noexcept {
        if(cursor.first() == '\r' && cursor.second() == '\n') {
            cursor.bump();
            cursor.bump();

            return make_token(Kind::NEWLINE, "\r\n", start, cursor.position());
        }

        if(cursor.first() == '\r') {
            cursor.bump();
            return make_token(Kind::NEWLINE, "\r", start, cursor.position());
        }

        cursor.bump();
        return make_token(Kind::NEWLINE, "\n", start, cursor.position());
    }

    [[nodiscard]] Token if_equal(Cursor &cursor, size_t start) noexcept {
        cursor.bump();

        if(cursor.first() == '=') {
            cursor.bump();

            return make_token(Kind::EQUAL_EQUAL, "==", start, cursor.position());
        }

        return make_token(Kind::EQUAL, "=", start, cursor.position());
    }

    [[nodiscard]] Token if_ident(Cursor &cursor, size_t start) noexcept {
        while(utils::is_ident_continue(cursor.first())) {
            cursor.bump();
        }

        StrView source_text = cursor.source_view.substr(start, cursor.position() - start);
        return make_token(Kind::IDENT, source_text, start, cursor.position());
    }

    [[nodiscard]] Token if_digit(Cursor &cursor, size_t start) noexcept {
        while(utils::is_digit(cursor.first())) {
            cursor.bump();
        }

        StrView source_text = cursor.source_view.substr(start, cursor.position() - start);
        return make_token(Kind::INT_LITERAL, source_text, start, cursor.position());
    }

    WHEEL_ALWAYS_INLINE_NODISCARD Token if_space(Cursor &cursor, size_t start) noexcept {
        cursor.bump();

        StrView source_text = _SOURCE_TEXT(cursor, start);
        return make_token(Kind::SPACE, source_text, start, cursor.position());
    }

    WHEEL_ALWAYS_INLINE_NODISCARD Token if_slash(Cursor &cursor, size_t start) noexcept {
        cursor.bump();

        const char second = cursor.first();
        if(second != '/' && second != '*') {
            
            return make_token(Kind::SLASH, _SOURCE_TEXT(cursor, start), start, cursor.position());
        }

        switch(second) {
            case '/': {
                const char third  = cursor.second();
                bool       is_doc = (third == '/');

                Kind       kind   = is_doc ? Kind::DOCUMENT_COMMENT : Kind::COMMENT;

                cursor.bump();
                if (is_doc) cursor.bump();

                while(!cursor.is_eof() && !properties::is_newline_like(cursor.first())) {
                    cursor.bump();
                }

                return make_token(kind, _SOURCE_TEXT(cursor, start), start, cursor.position());
            }

            case '*': {
                Kind kind = Kind::COMMENT;
                cursor.bump();

                while (!cursor.is_eof()) {
                    if (cursor.first() == '*' && cursor.second() == '/') {
                        cursor.bump();
                        cursor.bump();

                        break;
                    }

                    cursor.bump();
                }

                return make_token(kind, _SOURCE_TEXT(cursor, start), start, cursor.position());
            }
        }

        return make_token(Kind::ERROR, _SOURCE_TEXT(cursor, start), start, cursor.position());
    }
 
    constexpr std::array<TokenHandler, 256> HANDLERS = []() {
        std::array<TokenHandler, 256> table = {};

        /* Special */
        table['\t'] = if_tab;
        table['\r'] = if_newline;
        table['\n'] = if_newline;
        table[' ']  = if_space;
        table['/']  = if_slash;

        /* Operator(s) */
        table['=']  = if_equal;

        constexpr int IDENT_START_FLAG = -1;
        constexpr int DIGIT_FLAG       = -2;

        for(int character = 0; character < 256; character++) {
            if(utils::is_ident_start(static_cast<char>(character))) {
                table[character] = if_ident;
            }

            if (utils::is_digit(static_cast<char>(character))) {
                table[character] = if_digit;
            }
        }

        return table;
    }();

    [[nodiscard]] TokenHandler get_handler(char character) {
        unsigned char uc = static_cast<unsigned char>(character);

        return HANDLERS[uc];
    }
}

#endif // HANDLERS_HXX