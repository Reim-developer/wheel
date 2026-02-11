#if !defined(INT_DECLARATION_HXX)
#define INT_DECLARATION_HXX

#include <wheel_lexer/token.hxx>
#include "wheel_parser/config.hxx"
#include "symbol.hxx"

using namespace wheel_lexer;

WHEEL_PARSER_AST_NAMESPACE
    struct ASTIntDeclaration {
        SymbolID type_symbol;
        SymbolID variable_symbol;
        int64_t intializer_value;
        SourceLocation source_location;
    };

WHEEL_PARSER_END_NAMESPACE


#endif // INT_DECLARATION_HXX