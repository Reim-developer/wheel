#if !defined(UTILS_HXX)
#define UTILS_HXX

#include <string>
#include <string_view>
#include "aliases.hxx"
#include "__wheel_lexer_config.hxx"

WHEEL_LEXER_NAMESPACE

    using String         = std::string;
    using StrView        = std::string_view;

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_digit(char character) noexcept {
        return character >= '0' && character <= '9';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_whitespace(char character) noexcept {
        return character == ' ';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_lower(char character) noexcept {
        return character >= 'a' && character <= 'z';
    }
    
    WHEEL_COMPTIME_NODISCARD_INLINE bool is_upper(char character) noexcept {
        return character >= 'A' && character <= 'Z';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_alpha(char character) noexcept {
        return is_lower(character) || is_upper(character);
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_indent(char character) noexcept {
        return character == '\t';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_newline(char character) noexcept {
        return character == '\n' || character == '\r';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_hex_digit(char character) noexcept {
        return is_digit(character) ||
            (character >= 'a' && character <= 'f') ||
            (character >= 'A' && character <= 'F');
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_ident_start(char character) noexcept {
        return is_alpha(character) || character == '_';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_ident_continue(char character) noexcept {
        return is_ident_start(character) || is_digit(character);
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_special_ident(char character) noexcept {
        return character == 'r';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool starts_with(String str, StrView prefix) noexcept {
        return str.size() >= prefix.size() && 
            std::equal(prefix.begin(), prefix.end(), str.begin());
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool starts_with(StrView str, char prefix) noexcept {
        return !str.empty() && str[0] == prefix;
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_sign(char character) noexcept {
        return character == '-' || character == '+';
    }

    WHEEL_COMPTIME_NODISCARD_INLINE bool is_exponent_marker(char character) noexcept {
        return character == 'E' || character == 'e';
    }

END_NAMESPACE 

#endif // UTILS_HXX