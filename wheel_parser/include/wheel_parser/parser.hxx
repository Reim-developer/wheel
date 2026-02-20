#include "wheel_lexer/kind.hxx"
#if !defined(PARSER_HXX)
#define PARSER_HXX

#include <string_view>
#include "config.hxx"
#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/token.hxx>
#include <wheel_memory/allocator.hxx>
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/ast/symbol.hxx"

using wheel_lexer::Token;
using wheel_lexer::TokenKind;
using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::StringInterner;
using std::string_view;

WHEEL_PARSER_NAMESPACE
    class WheelParser {
        private:
            Lexer           &m_lexer;
            Arena           &m_arena;
            Token           m_current_token;
            StringInterner  &m_interner;

        private:
            void next_token() noexcept {
                m_current_token = m_lexer.next_token();
            }

            bool str_matches(string_view str) noexcept;
            bool token_matches(TokenKind token_kind) noexcept;

            const Token &consume() noexcept;
            const Token *copy_token() const noexcept;

        public:
            explicit WheelParser(Lexer &lexer, Arena &arena, StringInterner &interner) noexcept;
            VariableDeclaration *parse_variable_declaration() noexcept;
            void parse() noexcept;
    };

WHEEL_PARSER_END_NAMESPACE
#endif // PARSER_HXX