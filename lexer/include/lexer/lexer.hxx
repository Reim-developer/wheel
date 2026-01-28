#if !defined(LEXER_HXX)

#include "token.hxx"
#include "cursor.hxx"
#include "utils.hxx"
#include "handlers.hxx"
#include "aliases.hxx"

namespace lexer {
    class Lexer {
        private:
            Cursor          cursor;
            SourceLocation  source_location;

            void update_location(char character) {
                if (character == '\n') {
                    source_location.line++;
                    source_location.column = 1;
                } else {
                    source_location.column++;   
                }

                source_location.offset++;
            }

            void skip_whitespace() {
                while(!cursor.is_eof() && utils::is_whitespace(cursor.first())) {
                    update_location(cursor.first());
                    cursor.bump();
                }
            }

        public:
            explicit Lexer(StringView source_text) : 
                cursor(source_text), 
                source_location{1, 1, 0} {}

            [[nodiscard]] Token next_token() {
                skip_whitespace();

                if(cursor.is_eof()) {
                    return make_eof(cursor.position());
                }

                size_t start    = cursor.position();
                char character  = cursor.first();

                if (auto handler = get_handler(character)) {
                    Token token = handler(cursor, start);

                    for (size_t index = start; index < cursor.position(); index++) {
                        update_location(cursor.source_view[index]);
                    }

                    return Token {
                        token.kind, token.str,
                        token.start, token.end, source_location
                    };
                }

                update_location(character);
                cursor.bump();

                return make_error(start, StringView(&character, 1));
            }
    };
}

#endif // LEXER_HXX