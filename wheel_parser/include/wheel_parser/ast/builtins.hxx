#if !defined(WHEEL_PARSER_AST_BUILTINS_HXX)
#define WHEEL_PARSER_AST_BUILTINS_HXX

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/symbol.hxx"

WHEEL_PARSER_AST_NAMESPACE
    enum class BuiltinType : uint8_t {
        Int,
        String,
        Count
    };

    enum class BuiltinFunction : uint8_t {
        Println,
        Count
    };

    template <typename BuiltinEnum>
    struct BuiltinSpec {
        BuiltinEnum builtin;
        std::string_view text;
    };

    inline constexpr std::array<BuiltinSpec<BuiltinType>, static_cast<size_t>(BuiltinType::Count)>
        k_builtin_type_specs = {{
            {BuiltinType::Int, "int"},
            {BuiltinType::String, "string"},
        }};

    inline constexpr std::array<BuiltinSpec<BuiltinFunction>, static_cast<size_t>(BuiltinFunction::Count)>
        k_builtin_function_specs = {{
            {BuiltinFunction::Println, "println"},
        }};

    struct BuiltinRegistry {
        SymbolID int_type;
        SymbolID string_type;
        SymbolID println_function;

        [[nodiscard]] constexpr SymbolID type_symbol(BuiltinType type) const noexcept {
            switch (type) {
                case BuiltinType::Int:
                    return int_type;
                case BuiltinType::String:
                    return string_type;
                case BuiltinType::Count:
                    break;
            }

            return INVALID_SYMBOL_ID;
        }

        [[nodiscard]] constexpr SymbolID function_symbol(BuiltinFunction function) const noexcept {
            switch (function) {
                case BuiltinFunction::Println:
                    return println_function;
                case BuiltinFunction::Count:
                    break;
            }

            return INVALID_SYMBOL_ID;
        }

        [[nodiscard]] constexpr bool resolve_type(SymbolID symbol_id, BuiltinType &type) const noexcept {
            if (symbol_id == int_type) {
                type = BuiltinType::Int;
                return true;
            }

            if (symbol_id == string_type) {
                type = BuiltinType::String;
                return true;
            }

            return false;
        }

        [[nodiscard]] constexpr bool resolve_function(
            SymbolID symbol_id,
            BuiltinFunction &function
        ) const noexcept {
            if (symbol_id == println_function) {
                function = BuiltinFunction::Println;
                return true;
            }

            return false;
        }
    };

    [[nodiscard]] inline bool lookup_builtin_type(
        std::string_view text,
        BuiltinType &type
    ) noexcept {
        for (const auto &spec : k_builtin_type_specs) {
            if (spec.text == text) {
                type = spec.builtin;
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] inline bool lookup_builtin_function(
        std::string_view text,
        BuiltinFunction &function
    ) noexcept {
        for (const auto &spec : k_builtin_function_specs) {
            if (spec.text == text) {
                function = spec.builtin;
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] inline BuiltinRegistry intern_builtin_registry(StringInterner &interner) {
        return BuiltinRegistry{
            interner.intern(k_builtin_type_specs[0].text),
            interner.intern(k_builtin_type_specs[1].text),
            interner.intern(k_builtin_function_specs[0].text),
        };
    }
WHEEL_PARSER_END_NAMESPACE

#endif // WHEEL_PARSER_AST_BUILTINS_HXX
