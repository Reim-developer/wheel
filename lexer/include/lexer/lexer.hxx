#if !defined(LEXER_HXX)
#define LEXER_HXX

#include "token.hxx"
#include "cursor.hxx"
#include "utils.hxx"
#include "handlers.hxx"
#include "aliases.hxx"
#include "properties.hxx"

namespace properties = lexer::properties;
namespace lexer {
    class Lexer {
        private:

        private:
            Cursor          cursor;
            SourceLocation  source_location;

            void update_location(char character) {
                if(properties::is_newline_like(character)) {
                    source_location.line++;
                    source_location.column = 1;
                } else {
                    source_location.column++;
                }
            }

        public:
            explicit Lexer(StringView source_text) : 
                cursor(source_text), 
                source_location{1, 1, 0} {}

            WHEEL_ALWAYS_INLINE_NODISCARD Token next_token() {
                if(cursor.is_eof()) {
                    return make_eof(cursor.position());
                }

                size_t start    = cursor.position();
                char character  = cursor.first();

                if (auto handler = get_handler(character)) {
                    Token token = handler(cursor, start);

                    for(size_t index = 0; index < token.str.size(); ++index) {
                        char character = token.str[index];

                        char prev_char = (index > 0) ? token.str[index - 1] : '\0';
                        if(properties::is_crlf_sequence(character, prev_char)) {
                            source_location.column = 1;
                        } else {
                            update_location(character);
                        }
                    }
 
                    source_location.offset = start;

                    return Token {
                        token.kind, token.str,
                        token.start, token.end, source_location
                    };
                }

                update_location(character);
                cursor.bump();
                source_location.offset = start;

                return make_error(start, StringView(&character, 1));
            }
    };
}

#endif // LEXER_HXX