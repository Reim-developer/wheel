#if !defined(KEYWORDS_HXX)
#define KEYWORDS_HXX

#include "wheel_parser/config.hxx"
#include <cstdint>
#include <cstddef>
#include <array>
#include <string_view>

WHEEL_PARSER_AST_NAMESPACE
    enum class Keyword : uint8_t {
        Var,
        Int,
        String,
        Count
    };

    struct KeywordSpec {
        std::string_view text;
    };

    inline constexpr std::array<KeywordSpec, static_cast<size_t>(Keyword::Count)> k_keywords = {{
        {"var"},
        {"int"},
        {"string"}
    }};


WHEEL_PARSER_END_NAMESPACE



#endif // KEYWORDS_HXX
