#if !defined(WHEEL_SENEMATIC_SEMANTIC_HXX)
#define WHEEL_SENEMATIC_SEMANTIC_HXX

#include <cstddef>

#if !defined(WHEEL_EXPERIMENT) || !defined(WHEEL_SMALL_VEC)
    #include <vector>
#endif

#include <wheel_lexer/token.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_memory/vec.hxx>
#include <wheel_parser/ast/builtins.hxx>
#include <wheel_parser/ast/nodes.hxx>
#include <wheel_parser/ast/symbol.hxx>
#include "wheel_semantic/program.hxx"
#include "wheel_semantic/errors.hxx"
#include "config.hxx"

WHEEL_SEMANTIC_BEGIN_NAMESPACE
    using wheel_memory::Arena;
    using wheel_parser::ast::BuiltinRegistry;
    using wheel_parser::ast::StatementNode;
    using wheel_parser::ast::StringInterner;
    using wheel_parser::ast::SymbolID;
    using wheel_lexer::Token;
    using wheel_semantic::SemanticErrorCode;

    struct SemanticError {
        SemanticErrorCode code;
        const Token *token;
        const char *message;
    };

    struct DeclaredSymbol {
        SymbolID name;
        ValueKind type;
        const Token *token;
    };

    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        using SemanticErrorList = wheel_memory::SmallVec<SemanticError>;
        using SemanticProgramList = wheel_memory::SmallVec<const ExecutableStatement *>;
        using DeclaredSymbolList = wheel_memory::SmallVec<DeclaredSymbol>;
    #else
        using SemanticErrorList = std::vector<SemanticError>;
        using SemanticProgramList = std::vector<const ExecutableStatement *>;
        using DeclaredSymbolList = std::vector<DeclaredSymbol>;
    #endif

    class SemanticAnalyzer {
        private:
            Arena &m_arena;
            BuiltinRegistry m_builtins;
            SemanticErrorList m_errors;
            SemanticProgramList m_program;
            DeclaredSymbolList m_symbols;

        private:
            void push_error(SemanticErrorCode code, const Token *token) noexcept;
            bool analyze_statement(const StatementNode *statement) noexcept;
            bool analyze_expression_statement(
                const wheel_parser::ast::ExpressionStatement *statement
            ) noexcept;
            bool analyze_call_expression(
                const wheel_parser::ast::CallExpression *expression,
                const Token *statement_token
            ) noexcept;
            bool analyze_variable_declaration(
                const wheel_parser::ast::VariableDeclaration *declaration
            ) noexcept;
            const DeclaredSymbol *find_symbol(SymbolID name) const noexcept;
            bool resolve_type(SymbolID type_symbol, ValueKind &type_kind) const noexcept;
            bool parse_string_literal(
                const Token *token,
                std::string_view &value
            ) const noexcept;
            bool build_operand(
                const wheel_parser::ast::ExpressionNode *expression,
                Operand &operand,
                ValueKind &value_kind
            ) noexcept;
            bool count_format_placeholders(
                std::string_view format_string,
                size_t &placeholder_count
            ) const noexcept;

        public:
            explicit SemanticAnalyzer(Arena &arena, StringInterner &interner) noexcept;
            bool analyze(StatementNode *const *statements, size_t count) noexcept;
            void reset() noexcept;

            [[nodiscard]] ProgramView program() const noexcept;
            [[nodiscard]] size_t statement_count() const noexcept;
            [[nodiscard]] const ExecutableStatement *const *statements_data() const noexcept;

            [[nodiscard]] size_t error_count() const noexcept;
            [[nodiscard]] const SemanticError *errors_data() const noexcept;
    };

WHEEL_SEMANTIC_END_NAMESPACE

#endif // WHEEL_SENEMATIC_SEMANTIC_HXX

