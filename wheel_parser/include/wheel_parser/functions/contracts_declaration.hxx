#if !defined (PARSER_CONTRACTS_DECLARATION_HXX)
#define PARSER_CONTRACTS_DECLARATION_HXX
#include "wheel_parser/config.hxx"
#include "wheel_parser/declaration.hxx"
#include "wheel_parser/ast/symbol.hxx"
#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_lexer/token.hxx>

using wheel_memory::Arena;
using wheel_lexer::Lexer;
using wheel_lexer::Token;
using wheel_parser::ast::StringInterner;

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    #define ERROR_CONTEXT(code, error_message) EmitErrorConfig { \
        code, \
        contract.state.error_list, &contract.state.current_token, \
        contract.state.current_token.kind, \
        contract.lexer.get_source_location(contract.state.current_token), \
        contract.arena, \
        contract.state.current_token, contract.lexer, \
        error_message \
    }
    #define PARSE_BEGIN(func_name, return_type, ...) inline return_type* func_name(ParserFuncContract contract, __VA_ARGS__) noexcept {
    #define PARSE_END }
    #define IGNORE (void(0))

    struct ParserState {
        Token &current_token;
        ParserErrorList &error_list;
    };

    struct ParserFuncContract {
        Lexer &lexer;
        Arena &arena;
        StringInterner &interner;
        ParserState state;
    };


WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif // PARSER_CONTRACTS_DECLARATION_HXX