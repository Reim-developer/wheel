#if !defined(LEXER_HXX)
#define LEXER_HXX

#include "token.hxx"
#include "cursor.hxx"
#include "handlers.hxx"
#include "aliases.hxx"
#include "properties.hxx"
#include "source_map.hxx"
#include "__wheel_lexer_config.hxx"

WHEEL_LEXER_NAMESPACE
    class Lexer {
        private:

        private:
            Cursor          cursor;
            SourceMap       source_map;
            SourceLocation  source_location;

            void update_location(char character) {
                if (is_newline_like(character)) {
                    source_location.line++;
                    source_location.column = 1;
                } else {
                    source_location.column++;
                }
            }

        public:
            explicit Lexer(StringView source_text) : cursor(source_text), source_location {1, 1, 0} {
                source_map.build_table(source_text);
            }

            WHEEL_ALWAYS_INLINE_NODISCARD Token next_token() noexcept {
                if(cursor.is_eof()) {
                    return make_eof(cursor.position());
                }

                size_t start    = cursor.position();
                char character  = cursor.first();

                if (auto handler = get_handler(character)) {
                    Token token = handler(cursor, start);

                    for (size_t index = 0; index < token.str.size(); ++index) {
                        char character = token.str[index];
                        char prev_char = (index > 0) ? token.str[index - 1] : '\0';

                        if(is_crlf_sequence(character, prev_char)) {
                            source_location.column = 1;
                        } else {
                            update_location(character);
                        }
                    }

                    return token;
                }

                cursor.bump();
                return make_identifier_fallback(start, StringView(&character, 1));
            }

        WHEEL_ALWAYS_INLINE_NODISCARD SourceLocation get_source_location(const Token &token) noexcept {
            return source_map.source_location(token.start);
        }

        WHEEL_ALWAYS_INLINE_NODISCARD StringView get_source_line(size_t offset) const noexcept {
            return source_map.source_line(cursor.source_view, offset);
        }
    };

END_NAMESPACE

#endif // LEXER_HXX
