#if !defined (PARSER_DECLARATION_HXX)
#define PARSER_DECLARATION_HXX

#include <wheel_memory/vec.hxx>
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/error.hxx"

using wheel_memory::SmallVec;
using wheel_parser::ast::StatementNode;
using wheel_parser::ParseError;

#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
    using ParserErrorList = wheel_memory::SmallVec<ParseError>;
    using ParserStatementList = wheel_memory::SmallVec<StatementNode *>;
#else
    using ParserErrorList = std::vector<ParseError>;
    using ParserStatementList = std::vector<StatementNode *>;
#endif

#endif // PARSER_DECLARATION_HXX