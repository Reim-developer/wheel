#if !defined(WHEEL_INTERPRETER_HXX)
#define WHEEL_INTERPRETER_HXX

#include <cstddef>
#include <cstdint>

#if !defined(WHEEL_EXPERIMENT) || !defined(WHEEL_SMALL_VEC)
    #include <vector>
#endif

#include <wheel_lexer/token.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_memory/vec.hxx>
#include <wheel_parser/ast/symbol.hxx>
#include <wheel_semantic/program.hxx>

namespace wheel_interpreter {
    using wheel_memory::Arena;
    using wheel_parser::ast::SymbolID;
    using wheel_semantic::DefineGlobalStatement;
    using wheel_semantic::ExecutableStatement;
    using wheel_semantic::Operand;
    using wheel_semantic::PrintlnStatement;
    using wheel_semantic::ProgramView;
    using wheel_lexer::Token;

    using RuntimeValueKind = wheel_semantic::ValueKind;
    using RuntimeValue = wheel_semantic::Value;

    struct RuntimeBinding {
        SymbolID name;
        RuntimeValue value;
    };

    enum class RuntimeErrorCode : uint16_t {
        DuplicateBinding    = 4001,
        UnsupportedStatement = 4002,
        UnknownBinding      = 4003,
    };

    struct RuntimeError {
        RuntimeErrorCode code;
        const Token *token;
        const char *message;
    };

    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        using RuntimeBindingList = wheel_memory::SmallVec<RuntimeBinding>;
        using RuntimeErrorList = wheel_memory::SmallVec<RuntimeError>;
    #else
        using RuntimeBindingList = std::vector<RuntimeBinding>;
        using RuntimeErrorList = std::vector<RuntimeError>;
    #endif

    class WheelInterpreter {
        private:
            RuntimeBindingList m_bindings;
            RuntimeErrorList m_errors;

        private:
            void push_error(RuntimeErrorCode code, const Token *token) noexcept;
            bool execute_statement(const ExecutableStatement *statement) noexcept;
            bool resolve_operand(const Operand &operand, RuntimeValue &value) noexcept;
            bool execute_define_global(
                const DefineGlobalStatement *statement
            ) noexcept;
            bool execute_println(
                const PrintlnStatement *statement
            ) noexcept;
            const RuntimeBinding *find_binding(SymbolID name) const noexcept;

        public:
            explicit WheelInterpreter(Arena &arena) noexcept;
            bool execute(ProgramView program) noexcept;
            bool execute(const ExecutableStatement *const *statements, size_t count) noexcept;
            void reset() noexcept;

            size_t binding_count() const noexcept;
            const RuntimeBinding *bindings_data() const noexcept;
            const RuntimeValue *find_value(SymbolID name) const noexcept;

            size_t error_count() const noexcept;
            const RuntimeError *errors_data() const noexcept;
    };
} // namespace wheel_interpreter

#endif // WHEEL_INTERPRETER_HXX

