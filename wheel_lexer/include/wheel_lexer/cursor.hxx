#if !defined(CURSOR_HXX)
#define CURSOR_HXX

#include <string_view>
#include <cstddef>
#include <algorithm>
#include "aliases.hxx"
#include "__wheel_lexer_config.hxx"
#include <wheel_utils/logging.hxx>

WHEEL_LEXER_NAMESPACE

    constexpr char EOF_CHAR = '\0';
    struct Cursor {
        private:
            using StringView = std::string_view;
        
        public:
            StringView source_view;
            size_t pos      = 0;
            char prev       = EOF_CHAR;

            explicit Cursor(StringView input = {}) noexcept : source_view(input) {}
            
            [[nodiscard]] size_t len_remaining() const noexcept {
                return source_view.size() - std::min(pos, source_view.size());
            }

            [[nodiscard]] StringView remaining() const noexcept {
                return source_view.substr(pos);
            }

            [[nodiscard]] StringView slice(size_t start, size_t end) const noexcept {
                if (start >= source_view.size() || start > end) {
                    return {};
                }

                return source_view.substr(start, std::min(end - start, source_view.size()));
            }

            WHEEL_ALWAYS_INLINE_NODISCARD const char* chars() const noexcept {
                return source_view.data() + std::min(pos, source_view.size());
            }

            WHEEL_ALWAYS_INLINE_NODISCARD char previous() const noexcept {
                return prev;
            }

            WHEEL_ALWAYS_INLINE_NODISCARD bool has_previous() const noexcept {
                return prev != EOF_CHAR;
            }

            WHEEL_ALWAYS_INLINE_NODISCARD char first() const noexcept {
                return pos < source_view.size() ? source_view[pos] : EOF_CHAR;   
            }

            WHEEL_ALWAYS_INLINE_NODISCARD char second() const noexcept {
                return pos + 1 < source_view.size() ? source_view[pos + 1] : EOF_CHAR;
            }

            WHEEL_ALWAYS_INLINE_NODISCARD char third() const noexcept {
                return pos + 2 < source_view.size() ? source_view[pos + 2] : EOF_CHAR;
            }
            
            WHEEL_ALWAYS_INLINE void advance(size_t n_char = 1) noexcept {
                pos = std::min(pos + n_char, source_view.size());
            }

            [[nodiscard]] bool is_eof() const noexcept {
                return pos >= source_view.size();
            }

            [[nodiscard]] size_t position() const noexcept {
                return pos;
            }

            char bump() noexcept {
                if (pos >= source_view.size()) {
                    return EOF_CHAR;
                }

                prev = source_view[pos];
                return source_view[pos++];
            }
    };

END_NAMESPACE

#endif // CURSOR_HXX