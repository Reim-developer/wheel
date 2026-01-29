#if !defined(HANDLERS_HXX)
#define HANDLERs_HXX
#include "cursor.hxx"
#include "token.hxx"
#include "utils.hxx"
#include <string_view>
#include <array>

namespace lexer {
    using TokenHandler = Token(*)(Cursor&, size_t);
    using StrView      = std::string_view;
    using Kind         = TokenKind;

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

    constexpr std::array<TokenHandler, 256> HANDLERS = []() {
        std::array<TokenHandler, 256> table = {};

        /* Special */
        table['\t'] = if_tab;
        table['\r'] = if_newline;
        table['\n'] = if_newline;

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