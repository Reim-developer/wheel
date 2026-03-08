#if !defined(PARSER_HXX)
#define PARSER_HXX

#if !defined (WHEEL_EXPERIMENT) && !defined (WHEEL_SMALL_VEC)
    #include <vector>
#endif 

#include <string_view>
#include "config.hxx"
#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/token.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_memory/vec.hxx>
#include "wheel_parser/error.hxx"
#include "wheel_parser/ast/builtins.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/ast/symbol.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include "wheel_parser/declaration.hxx"

using wheel_lexer::Token;
using wheel_lexer::TokenKind;
using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::BuiltinRegistry;
using wheel_parser::ast::StringInterner;
using wheel_parser::ast::Keyword;
using std::string_view;

WHEEL_PARSER_NAMESPACE
    class WheelParser {
        public:
            Token           m_current_token;
            ParserErrorList m_errors;
            BuiltinRegistry m_builtins;

        private:
            Lexer               &m_lexer;
            Arena               &m_arena;
            StringInterner      &m_interner;
            ParserStatementList m_statements;

        private:
            void synchronize_statement() noexcept;
            wheel_parser::ast::ExpressionNode *parse_expression() noexcept;
            StatementNode *parse_call_statement() noexcept;
            StatementNode *parse_statement() noexcept;

        public:
            explicit WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept;
            void parse() noexcept;
            size_t error_count() const noexcept;
            const ParseError *errors_data() const noexcept;
            size_t statement_count() const noexcept;
            StatementNode *const *statements_data() const noexcept;
            void clear_errors() noexcept;
            ParseDiagnosticList collect_diagnostics(std::string_view file_name = {}) const;

            [[nodiscard]]
            Token *get_current_token() noexcept;
    };

WHEEL_PARSER_END_NAMESPACE
#endif // PARSER_HXX
