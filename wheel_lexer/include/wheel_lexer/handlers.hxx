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
    #define _WHEEL_MAKE_TOKEN(kind, source, label) \
        do { \
            DEBUG_PRINT(FORMAT("'{}': {}", #label, source)); \
            return make_token(kind, source, start, cursor.position()); \
        } while(0)
    #define _WHEEL_CONSUME_ALL_IF(condition, cursor) \
        do { \
            while(condition) cursor.bump(); \
        } while(0)
  
    using TokenHandler = Token(*)(Cursor&, size_t);
    using StrView      = std::string_view;
    using Kind         = TokenKind;

    _WHEEL_HANDLERS(if_tab) {
        cursor.bump(); 
        return make_token(Kind::TAB, "\t", start, cursor.position());
    }

    _WHEEL_HANDLERS(if_string_literal) {
        cursor.bump();

        while (true) {
            auto closed_str = cursor.bump();

            if(is_newline_like(closed_str)) break;
            if(closed_str == '\"' || closed_str == '\0') break;
        }

        _WHEEL_MAKE_TOKEN(TokenKind::STRING_LITERAL, _SOURCE_TEXT(cursor, start), if_string_literal);
    }

    _WHEEL_HANDLERS(if_raw_string_literal) {
        cursor.bump();

        if (cursor.first() == '\"') {
            cursor.bump();

            while(true) {
                auto closed_str = cursor.bump();
                if (closed_str == '\"' || is_eof(closed_str)) break;
            }

            _WHEEL_MAKE_TOKEN(TokenKind::RAW_STRING_LITERAL, _SOURCE_TEXT(cursor, start), if_string_literal);
        }

        _WHEEL_MAKE_TOKEN(TokenKind::IDENT, _SOURCE_TEXT(cursor, start), if_string_literal);
    }

    _WHEEL_HANDLERS(if_newline) {
        if(cursor.first() == '\r' && cursor.second() == '\n') [[likely]] {
            cursor.bump();
            cursor.bump();

            return make_token(Kind::NEWLINE, "\r\n", start, cursor.position());
        }

        if(cursor.first() == '\r') [[likely]] {
            cursor.bump();
            return make_token(Kind::NEWLINE, "\r", start, cursor.position());
        }

        cursor.bump();
        return make_token(Kind::NEWLINE, "\n", start, cursor.position());
    }

    _WHEEL_HANDLERS(if_equal) {
        cursor.bump();

        auto next = cursor.first();
        switch(next) {
            case '=': [[likely]] {
                cursor.bump();
                _WHEEL_MAKE_TOKEN(Kind::EQUAL_EQUAL, _SOURCE_TEXT(cursor, start), if_equal);
            }

            case '>': [[likely]] {
                cursor.bump();
                _WHEEL_MAKE_TOKEN(Kind::FAT_ARROW, _SOURCE_TEXT(cursor, start), if_equal);
            }
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
        _WHEEL_CONSUME_ALL_IF(is_digit(cursor.first()), cursor);

        if (cursor.first() ==  '.' && is_digit(cursor.second())) {
            cursor.bump();

            _WHEEL_CONSUME_ALL_IF(is_digit(cursor.first()), cursor);
            if(is_exponent_marker(cursor.first())) {
                cursor.bump();

                _WHEEL_CONSUME_ALL_IF(is_sign(cursor.first()), cursor);
                _WHEEL_CONSUME_ALL_IF(is_digit(cursor.first()), cursor);
            }

            _WHEEL_MAKE_TOKEN(Kind::FLOAT_LITERAL, _SOURCE_TEXT(cursor, start), if_digit);
        }

        if(is_exponent_marker(cursor.first())) {
            cursor.bump();

            _WHEEL_CONSUME_ALL_IF(is_sign(cursor.first()), cursor);
            _WHEEL_CONSUME_ALL_IF(is_digit(cursor.first()), cursor);
            _WHEEL_MAKE_TOKEN(Kind::FLOAT_LITERAL, _SOURCE_TEXT(cursor, start), if_digit);
        }

        _WHEEL_MAKE_TOKEN(Kind::INT_LITERAL, _SOURCE_TEXT(cursor, start), if_digit);
    }

    _WHEEL_HANDLERS(if_space) {
        cursor.bump();

        return make_token(Kind::SPACE, _SOURCE_TEXT(cursor, start), start, cursor.position());
    }

    _WHEEL_HANDLERS(if_slash) {
        cursor.bump();

        const char second = cursor.first();
        if(second != '/' && second != '*') [[likely]] {
            _WHEEL_MAKE_TOKEN(Kind::SLASH, _SOURCE_TEXT(cursor, start), if_slash);
        }

        switch(second) {
            case '/': {
                const char third  = cursor.second();
                if WHEEL_STR_LIKELY(third, '/') {
                    cursor.bump();
                    
                    _WHEEL_CONSUME_ALL_IF(
                        !cursor.is_eof() && !is_newline_like(cursor.first()),
                        cursor
                    );
                    _WHEEL_MAKE_TOKEN(Kind::DOCUMENT_COMMENT, _SOURCE_TEXT(cursor, start), if_slash);
                }
            }

            case '*': {
                cursor.bump();

                while (!cursor.is_eof()) {
                    const char first  = cursor.first();
                    const char second = cursor.second();

                    if (first == '/' && second == '*') [[likely]] {
                        cursor.bump();
                        cursor.bump();

                        break;
                    }

                    cursor.bump();
                }
                
                _WHEEL_MAKE_TOKEN(Kind::COMMENT, _SOURCE_TEXT(cursor, start), if_slash);
            }
        }

       _WHEEL_MAKE_TOKEN(Kind::IDENT, _SOURCE_TEXT(cursor, start), if_slash);
    }

    _WHEEL_HANDLERS(if_plus) {
        cursor.bump();

        auto next = cursor.first();
        if WHEEL_STR_LIKELY(next, '+') {
            cursor.bump();
            _WHEEL_MAKE_TOKEN(Kind::PLUS_PLUS, _SOURCE_TEXT(cursor, start), if_plus);
        }
        _WHEEL_MAKE_TOKEN(Kind::PLUS, _SOURCE_TEXT(cursor, start), if_plus);
    }

    _WHEEL_HANDLERS(if_minus) {
        cursor.bump();
        
        auto next  = cursor.first();
        switch(next) {
            case '-': { 
                cursor.bump(); 
                _WHEEL_MAKE_TOKEN(Kind::MINUS_MINUS, _SOURCE_TEXT(cursor, start), if_minus); 
            }

            case '>': {
                cursor.bump();
                _WHEEL_MAKE_TOKEN(Kind::ARROW, _SOURCE_TEXT(cursor, start), if_minus);
            }
        }
    
        _WHEEL_MAKE_TOKEN(Kind::MINUS, _SOURCE_TEXT(cursor, start), if_minus);
    }

    _WHEEL_HANDLERS(if_star) {
        cursor.bump();

        _WHEEL_MAKE_TOKEN(Kind::STAR, _SOURCE_TEXT(cursor, start), if_star);
    }
 
    constexpr std::array<TokenHandler, 256> HANDLERS = []() {
        std::array<TokenHandler, 256> table = {};

        /* Special */
        table['\t'] = if_tab;
        table['\r'] = if_newline;
        table['\n'] = if_newline;
        table[' ']  = if_space;
        table['/']  = if_slash;

        table['\"'] = if_string_literal;
        table['r']  = if_raw_string_literal;

        /* Operator(s) */
        table['=']  = if_equal;
        table['+']  = if_plus;
        table['-']  = if_minus;
        table['*']  = if_star;

        for(int character = 0; character < 256; character++) {
            char char_str = static_cast<char>(character);

            if(is_ident_start(char_str) && !is_special_ident(char_str)) {
                table[character] = if_ident;
            }

            if (is_digit(char_str)) {
                table[character] = if_digit;
            }
        }

        return table;
    }();

    [[nodiscard]] inline TokenHandler get_handler(char character) {
        unsigned char uc = static_cast<unsigned char>(character);

        return HANDLERS[uc];
    }

END_NAMESPACE

#endif // HANDLERS_HXX