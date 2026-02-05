#if !defined(HANDLERS_HXX)
#define HANDLERS_HXX

#include <string_view>
#include <array>
#include "cursor.hxx"
#include "token.hxx"
#include "utils.hxx"
#include "aliases.hxx"
#include "properties.hxx"
#include <wheel_utils/logging.hxx>

WHEEL_LEXER_NAMESPACE

    #define _SOURCE_TEXT(cursor, start) cursor.source_view.substr(start, cursor.position() - start)
    #define _WHEEL_HANDLERS(func_name) WHEEL_ALWAYS_INLINE_NODISCARD Token func_name(Cursor &cursor, size_t start) noexcept
    #define _WHEEL_MAKE_TOKEN(kind, source) return make_token(kind, source, start, cursor.position())
  
    using TokenHandler = Token(*)(Cursor&, size_t);
    using StrView      = std::string_view;
    using Kind         = TokenKind;

    _WHEEL_HANDLERS(if_tab) {
        cursor.bump();
        return make_token(Kind::TAB, "\t", start, cursor.position());
    }

    _WHEEL_HANDLERS(if_newline) {
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

    _WHEEL_HANDLERS(if_equal) {
        cursor.bump();

        if(cursor.first() == '=') {
            cursor.bump();

            return make_token(Kind::EQUAL_EQUAL, "==", start, cursor.position());
        }

        return make_token(Kind::EQUAL, "=", start, cursor.position());
    }

    _WHEEL_HANDLERS(if_ident) {
        while(is_ident_continue(cursor.first())) {
            cursor.bump();
        }

        return make_token(Kind::IDENT, _SOURCE_TEXT(cursor, start), start, cursor.position());
    }

    _WHEEL_HANDLERS(if_digit) {
        while(is_digit(cursor.first())) {
            cursor.bump();
        }

        return make_token(Kind::INT_LITERAL, _SOURCE_TEXT(cursor, start), start, cursor.position());
    }

    _WHEEL_HANDLERS(if_space) {
        cursor.bump();

        return make_token(Kind::SPACE, _SOURCE_TEXT(cursor, start), start, cursor.position());
    }

    _WHEEL_HANDLERS(if_slash) {
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

                while(!cursor.is_eof() && !is_newline_like(cursor.first())) {
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

    _WHEEL_HANDLERS(if_plus) {
        cursor.bump();

        auto next = cursor.first();
        if(next == '+') {
            cursor.bump();
            
            DEBUG_PRINT(FORMAT("'if_plus': {}", next));
            _WHEEL_MAKE_TOKEN(Kind::PLUS_PLUS, _SOURCE_TEXT(cursor, start));
        }

        DEBUG_PRINT(FORMAT("'if_plus': {}", _SOURCE_TEXT(cursor, start)));
        _WHEEL_MAKE_TOKEN(Kind::PLUS, _SOURCE_TEXT(cursor, start));
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
        table['+']  = if_plus;

        for(int character = 0; character < 256; character++) {
            if(is_ident_start(static_cast<char>(character))) {
                table[character] = if_ident;
            }

            if (is_digit(static_cast<char>(character))) {
                table[character] = if_digit;
            }
        }

        return table;
    }();

    [[nodiscard]] TokenHandler get_handler(char character) {
        unsigned char uc = static_cast<unsigned char>(character);

        return HANDLERS[uc];
    }

END_NAMESPACE

#endif // HANDLERS_HXX