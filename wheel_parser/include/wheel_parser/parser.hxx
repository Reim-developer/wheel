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
    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        using ParserErrorList = wheel_memory::SmallVec<ParseError>;
        using ParserStatementList = wheel_memory::SmallVec<StatementNode *>;
    #else
        using ParserErrorList = std::vector<ParseError>;
        using ParserStatementList = std::vector<StatementNode *>;
    #endif

    class WheelParser {
        private:
            Lexer           &m_lexer;
            Arena           &m_arena;
            Token           m_current_token;
            StringInterner  &m_interner;
            BuiltinRegistry m_builtins;
            ParserErrorList m_errors;
            ParserStatementList m_statements;

        private:
            void skip_spaces() noexcept;
            void synchronize_statement() noexcept;
            StatementNode *emit_error(ParseErrorCode code) noexcept;
            const Token *copy_token() const noexcept;
            wheel_parser::ast::ExpressionNode *parse_expression() noexcept;
            StatementNode *parse_call_statement() noexcept;
            StatementNode *parse_statement() noexcept;

        public:
            explicit WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept;
            StatementNode *parse_variable_declaration() noexcept;
            void parse() noexcept;
            size_t error_count() const noexcept;
            const ParseError *errors_data() const noexcept;
            size_t statement_count() const noexcept;
            StatementNode *const *statements_data() const noexcept;
            void clear_errors() noexcept;
            ParseDiagnosticList collect_diagnostics(std::string_view file_name = {}) const;
    };

WHEEL_PARSER_END_NAMESPACE
#endif // PARSER_HXX
