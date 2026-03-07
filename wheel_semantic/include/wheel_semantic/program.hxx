#if !defined(WHEEL_SENEMATIC_PROGRAM_HXX)
#define WHEEL_SENEMATIC_PROGRAM_HXX

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <utility>

#if !defined(WHEEL_EXPERIMENT) || !defined(WHEEL_SMALL_VEC)
    #include <vector>
#endif

#include <wheel_lexer/token.hxx>
#include <wheel_memory/vec.hxx>
#include <wheel_parser/ast/symbol.hxx>
#include "config.hxx"

WHEEL_SEMANTIC_BEGIN_NAMESPACE

    using wheel_lexer::Token;
    using wheel_parser::ast::SymbolID;

    enum class ValueKind : uint8_t {
        Int,
        String
    };

    struct Value {
        ValueKind kind;
        int64_t int_value;
        std::string_view string_value;

        [[nodiscard]] static constexpr Value from_int(int64_t value) noexcept {
            return Value{
                ValueKind::Int,
                value,
                {}
            };
        }

        [[nodiscard]] static constexpr Value from_string(std::string_view value) noexcept {
            return Value{
                ValueKind::String,
                0,
                value
            };
        }
    };

    enum class OperandKind : uint8_t {
        Constant,
        Binding
    };

    struct Operand {
        OperandKind kind;
        const Token *token;
        Value constant;
        SymbolID binding;

        [[nodiscard]] static constexpr Operand from_constant(
            const Token *operand_token,
            Value value
        ) noexcept {
            return Operand{
                OperandKind::Constant,
                operand_token,
                value,
                wheel_parser::ast::INVALID_SYMBOL_ID
            };
        }

        [[nodiscard]] static constexpr Operand from_binding(
            const Token *operand_token,
            SymbolID symbol
        ) noexcept {
            return Operand{
                OperandKind::Binding,
                operand_token,
                Value::from_int(0),
                symbol
            };
        }
    };

    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        using OperandList = wheel_memory::SmallVec<Operand>;
    #else
        using OperandList = std::vector<Operand>;
    #endif

    enum class ExecutableStatementKind : uint8_t {
        DefineGlobal,
        Println
    };

    struct ExecutableStatement {
        ExecutableStatementKind kind;
        const Token *token;

        protected:
            explicit constexpr ExecutableStatement(
                ExecutableStatementKind statement_kind,
                const Token *statement_token
            ) noexcept :
                kind(statement_kind),
                token(statement_token) {
            }
    };

    struct DefineGlobalStatement final : ExecutableStatement {
        SymbolID name;
        Operand initializer;

        explicit constexpr DefineGlobalStatement(
            const Token *statement_token,
            SymbolID variable_name,
            Operand initial_value
        ) noexcept :
            ExecutableStatement(ExecutableStatementKind::DefineGlobal, statement_token),
            name(variable_name),
            initializer(initial_value) {
        }
    };

    struct PrintlnStatement final : ExecutableStatement {
        std::string_view format_string;
        OperandList arguments;

        explicit PrintlnStatement(
            const Token *statement_token,
            std::string_view format_text,
            OperandList &&print_arguments
        ) noexcept :
            ExecutableStatement(ExecutableStatementKind::Println, statement_token),
            format_string(format_text),
            arguments(std::move(print_arguments)) {
        }
    };

    struct ProgramView {
        const ExecutableStatement *const *statements;
        size_t count;
    };

WHEEL_SEMANTIC_END_NAMESPACE

#endif // WHEEL_SENEMATIC_PROGRAM_HXX
