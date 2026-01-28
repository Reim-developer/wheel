#if !defined(UTILS_HXX)
#define UTILS_HXX

#include <string>
#include <string_view>
#include <cctype>

namespace lexer::utils {
    using String         = std::string;
    using StrView        = std::string_view;

    [[nodiscard]] constexpr bool is_digit(char character) noexcept {
        return character >= '0' && character <= '9';
    }

    [[nodiscard]] constexpr bool is_whitespace(char character) noexcept {
        return character == ' ';
    }

    [[nodiscard]] constexpr bool is_lower(char character) noexcept {
        return character >= 'a' && character <= 'z';
    }
    
    [[nodiscard]] constexpr bool is_upper(char character) noexcept {
        return character >= 'A' && character <= 'Z';
    }

    [[nodiscard]] constexpr bool is_alpha(char character) noexcept {
        return is_lower(character) || is_upper(character);
    }

    [[nodiscard]] constexpr bool is_indent(char character) noexcept {
        return character == '\t';
    }

    [[nodiscard]] constexpr bool is_newline(char character) noexcept {
        return character == '\n' || character == '\r';
    }

    [[nodiscard]] constexpr bool is_hex_digit(char character) noexcept {
        return is_digit(character) ||
            (character >= 'a' && character <= 'f') ||
            (character >= 'A' && character <= 'F');
    }

    [[nodiscard]] constexpr bool is_ident_start(char character) noexcept {
        return is_alpha(character) || character == '_';
    }

    [[nodiscard]] constexpr bool is_ident_continue(char character) noexcept {
        return is_ident_start(character) || is_digit(character);
    }

    [[nodiscard]] constexpr bool starts_with(String str, StrView prefix) noexcept {
        return str.size() >= prefix.size() && 
            std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    [[nodiscard]] constexpr bool starts_with(StrView str, char prefix) noexcept {
        return !str.empty() && str[0] == prefix;
    }
}

#endif // UTILS_HXX