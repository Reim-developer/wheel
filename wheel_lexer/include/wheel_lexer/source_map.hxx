#if !defined(SOURCE_MAP_HXX)
#define SOURCE_MAP_HXX

#include <vector>
#include <algorithm>
#include "aliases.hxx"
#include "token.hxx"
#include "properties.hxx"

WHEEL_LEXER_NAMESPACE
    struct SourceMap {
        private:
           std::vector<uint32_t> line_starts;

        public:
            SourceMap () {
                line_starts.push_back(0);
            }
           
            WHEEL_ALWAYS_INLINE void build_table(StringView source) noexcept {
                line_starts.clear();
                line_starts.push_back(0);

                for (size_t index = 0; index < source.size(); ++index) {
                    char character = source[index];

                    if(is_crlf_sequence(character, (index > 0) ? source[index -1] : '\0' )) {
                        continue;
                    }

                    if(is_newline_like(character)) {
                        line_starts.push_back(static_cast<uint32_t>(index + 1));
                    }
                }
            }

            WHEEL_ALWAYS_INLINE_NODISCARD SourceLocation source_location(size_t offset) const {
                if(offset > UINT32_MAX) {
                    return SourceLocation {0, 0, offset};
                }

                uint32_t safe_offset = static_cast<uint32_t>(offset);
                auto it              = std::upper_bound(line_starts.begin(), line_starts.end(), safe_offset);

                if (it != line_starts.begin()) {
                    --it;
                } 

                size_t line_index           = std::distance(line_starts.begin(), it);
                uint32_t line_start_offset  = line_starts[line_index];
                uint32_t column             = (safe_offset - line_start_offset) + 1;

                return SourceLocation {
                    .line   = line_index + 1,
                    .column = column,
                    .offset = safe_offset
                };
            }

            WHEEL_ALWAYS_INLINE_NODISCARD StringView source_line(StringView source, size_t offset) const noexcept {
                if (line_starts.empty() || source.empty()) {
                    return {};
                }

                const auto capped_offset = std::min(offset, source.size());
                const uint32_t safe_offset = (capped_offset > UINT32_MAX)
                    ? UINT32_MAX
                    : static_cast<uint32_t>(capped_offset);

                auto it = std::upper_bound(line_starts.begin(), line_starts.end(), safe_offset);
                if (it != line_starts.begin()) {
                    --it;
                }

                const size_t line_index = static_cast<size_t>(std::distance(line_starts.begin(), it));
                const size_t start = line_starts[line_index];

                size_t end = source.size();
                if (line_index + 1 < line_starts.size()) {
                    end = line_starts[line_index + 1];
                }

                while (end > start) {
                    const char trailing = source[end - 1];
                    if (trailing != '\n' && trailing != '\r') {
                        break;
                    }

                    --end;
                }

                return source.substr(start, end - start);
            }
    };

END_NAMESPACE

#endif // SOURCE_MAP_HXX
