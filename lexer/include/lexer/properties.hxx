#if !defined(PROPERTIES_HXX)
#define PROPERTIES_HXX

#include <cstdint>
#include <array>

namespace lexer {
    enum CharFlags : uint8_t {
        NONE       = 0,
        NEWLINE    = 1 << 0,
        IS_CR      = 1 << 1,
        PREV_WAS_CR  = 1 << 2
    };

    constexpr std::array<uint8_t, 256> CHAR_PROPERTIES = []() {
        std::array<uint8_t, 256> properties = {};

        properties['\n'] = NEWLINE | PREV_WAS_CR;
        properties['\r'] = NEWLINE | IS_CR;

        return properties;
    }();

    [[nodiscard]] constexpr bool is_newline_like(char character) noexcept {
        return CHAR_PROPERTIES[static_cast<unsigned char>(character)] & (NEWLINE);
    }

    [[nodiscard]] constexpr bool is_crlf_sequence(char current_char, char prev_char) noexcept {
        return 
            (CHAR_PROPERTIES[static_cast<unsigned char>(current_char)] & PREV_WAS_CR) &&
            (CHAR_PROPERTIES[static_cast<unsigned char>(prev_char)] & IS_CR);
    }
}

#endif // PROPERTES_HXX