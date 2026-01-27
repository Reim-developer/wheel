#if !defined(LEXER_HXX)
#define LEXER_HXX
#include <__internal_headers/__config.hxx>
#include "token.hxx"
#include "utils.hxx"

using std::string_view;

using wheel::ast::lexer::Token;
using wheel::ast::lexer::is_digit;
using wheel::ast::lexer::is_identifier_start;
using wheel::ast::lexer::is_identifier_continue;
using wheel::ast::lexer::is_whitespace;
using wheel::ast::lexer::keyword_type;

WHEEL_AST_NAMESPACE :: lexer {
    class WheelLexer {
        private:
            const char *ptr;
            size_t line   = 1;
            size_t column = 1;

        private:
            void advance() {
                if(*ptr == '\n') {
                    line++; column = 1;
                } else {
                    column++;
                }

                ptr++;
            }

            Token make_token(TokenType token_type, string_view val = "") {
                return {token_type, val, line, column};
            }

            Token scan_whitespace() {
                const char *start = ptr;

                while(is_whitespace(*ptr)) {
                    if (*ptr == '\n') {
                        line++; column = 1;
                    } else {
                        column++;
                    }

                    ptr++; column++;
                }

                return make_token(
                    TokenType::WHITESPACE,
                    string_view(start, ptr - start)
                );
            }

            Token scan_number() {
                const char *start = ptr;
                while(is_digit(*ptr)) {
                    advance();
                }

                return make_token(
                    TokenType::NUMBER,
                    string_view(start, ptr - start)
                );
            }

            Token scan_identifier() {
                const char *start = ptr;
                while(is_identifier_continue(*ptr)) {
                    advance();
                }

                string_view ident(start, ptr - start);
                TokenType token_type = keyword_type(ident);

                return make_token(token_type, ident);
            }

        public:
            explicit WheelLexer(const char *source) : ptr(source) {}

            Token next_token() {
                if (*ptr == '\0') {
                    return make_token(TokenType::END_OF_LINE);
                }

                char character = *ptr;
                if(is_whitespace(character)) {
                    return scan_whitespace();
                }

                if(is_digit(character)) {
                    return scan_number();
                }

                if(is_identifier_start(character)) {
                    return scan_identifier();
                }

                switch (character) {
                    case '=': advance(); return make_token(TokenType::EQUAL);
                }

                advance();

                return make_token(TokenType::END_OF_LINE);
            }
    };
}

#endif // LEXER_HXX