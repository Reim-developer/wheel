#if !defined (FUNCTIONS_DECLARATION_HXX)
#define FUNCTIONS_DECLARATION_HXX
#include "contracts_declaration.hxx"
#include "expression_declaration.hxx"
#include "wheel_parser/parser_utils.hxx"
#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/ast/symbol.hxx"
#include "wheel_parser/functions/emit_error.hxx"
#include "wheel_parser/ast/keywords.hxx"
#include <wheel_lexer/kind.hxx>
#include <wheel_memory/vec.hxx>
#include <wheel_utils/logging.hxx>

WHEEL_PARSER_FUNCTIONS_BEGIN_NAMESPACE
    using wheel_parser::ast::StatementNode;
    using wheel_parser::ast::SymbolID;
    using wheel_parser::ast::Keyword;
    using wheel_parser::ast::FunctionDeclaration;
    using wheel_parser::ast::CallExpression;
    using wheel_parser::ast::ExpressionNode;
    using wheel_parser::ast::IdentifierExpression;
    using wheel_parser::ast::ExpressionStatement;
    using wheel_parser::functions::parse_expression_declaration;
    using wheel_parser::functions::ParserFuncContract;
    using wheel_parser::ParseErrorCode;
    using wheel_lexer::TokenKind;

    #if defined (WHEEL_EXPERIMENT) && defined (WHEEL_SMALL_VEC)
        using wheel_memory::SmallVec;
        using FuncArgs = SmallVec<ExpressionNode*>;

    #else 
        using FuncArgs = std::vector<ExpressionNode*>;
    #endif 

    inline void parse_function_argument(ParserFuncContract contract, FuncArgs &arguments) {
        while(true) {
            ExpressionNode *argument = parse_expression_declaration(contract);

            arguments.push_back(argument);
            skip_spaces(contract.lexer, contract.state.current_token);


            if (!token_matches(contract.state.current_token.kind, TokenKind::LEFT_PARENT)) {
                abort();
            }

            if (token_matches(contract.state.current_token.kind, TokenKind::RIGHT_PARENT)) {
                break;
            }
        }
    }

    /// TODO !!! Implement parse for functiond declaration.
    PARSE_BEGIN(parse_function_declaration, StatementNode)
        consume(contract.lexer, contract.state.current_token);
        auto current_token = contract.state.current_token;
        auto arena = &contract.arena;
        auto interner = &contract.interner;

        if (!token_matches(current_token.kind, TokenKind::IDENT)) {
            return nullptr;
        }

        /* Matches if
            function foo() -> void: ...
        */
        if (!keyword_matches(current_token, Keyword::Function)) {
            return nullptr;
        }

        skip_spaces(contract.lexer, current_token);
        const auto func_token = copy_token(*arena, current_token);

        if (matches_any_keywords(*func_token)) {
            return emit_error(ERROR_CONTEXT(ParseErrorCode::FunctionNameNotAllowed));
        }

        return nullptr;
    PARSE_END 
    
WHEEL_PARSER_FUNCTIONS_END_NAMESPACE

#endif // FUNCTIONS_DECLARATION_HXX