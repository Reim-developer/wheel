#include "wheel_parser/functions/contracts_declaration.hxx"
#include <__internal/__assert.hxx>

#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_parser/functions/functions_declaration.hxx>

using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::CallExpression;
using wheel_parser::ast::ExpressionStatement;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::StringInterner;
using wheel_parser::functions::ParserFuncContract;
using wheel_parser::functions::ParserState;
using wheel_parser::functions::parse_function_declaration;
// using wheel_parser::ast::from_node_kind;

TEST(test_parser_function_definition)
    using Contract = ParserFuncContract;

    Arena arena;
    Lexer lexer("function function() -> void:");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);

    auto parse_state =  ParserState {
        .current_token = parser.m_current_token,
        .error_list = parser.m_errors
    };
    auto result = parse_function_declaration(Contract {
        .lexer      = lexer,
        .arena      = arena,
        .interner   = interner,
        .state      = parse_state
    });
    if (!result) {
        DEBUG_OUTPUT(FORMAT("nullptr cmnr em zai"));
        return;
    }

    DEBUG_OUTPUT(FORMAT("Deo nullptr dau em zai oi, bo thach may segfault duoc"));
    DEBUG_OUTPUT(FORMAT("{}", parse_state.error_list.data()[0].message));

    // DEBUG_OUTPUT(FORMAT("{}", from_node_kind(result->node_kind)));
DONE 

TEST(test_parser_parses_println_with_dot_separator)
    Arena arena;
    Lexer lexer("println(\"{}\". name)");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);
    assert_eq(parser.statement_count(), 1);

    const auto *statement = static_cast<const ExpressionStatement *>(parser.statements_data()[0]);
    assert_eq(statement->node_kind, NodeKind::ExpressionStatement);
    assert_eq(statement->expression->node_kind, NodeKind::CallExpression);

    const auto *call = static_cast<const CallExpression *>(statement->expression);
    assert_eq(call->callee, interner.intern("println"));
    assert_eq(call->arguments.size(), 2);
    assert_eq(call->arguments.data()[0]->token->str, "\"{}\"");
    assert_eq(call->arguments.data()[1]->token->str, "name");
DONE

TEST(test_parser_parses_println_with_comma_separator)
    Arena arena;
    Lexer lexer("println(\"{}\", name)");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);
    assert_eq(parser.statement_count(), 1);

    const auto *statement = static_cast<const ExpressionStatement *>(parser.statements_data()[0]);
    const auto *call = static_cast<const CallExpression *>(statement->expression);
    assert_eq(call->arguments.size(), 2);
DONE

TEST(test_parser_parses_println_with_multiple_arguments)
    Arena arena;
    Lexer lexer("println(\"{} {} {}\". first. second. third)");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);
    assert_eq(parser.statement_count(), 1);

    const auto *statement = static_cast<const ExpressionStatement *>(parser.statements_data()[0]);
    const auto *call = static_cast<const CallExpression *>(statement->expression);
    assert_eq(call->arguments.size(), 4);
    assert_eq(call->arguments.data()[0]->token->str, "\"{} {} {}\"");
    assert_eq(call->arguments.data()[1]->token->str, "first");
    assert_eq(call->arguments.data()[2]->token->str, "second");
    assert_eq(call->arguments.data()[3]->token->str, "third");
DONE

/// !!! Fix all test  before un comment.
/// TODO: Fix @function_declaration.
TEST_MAIN
    // RUN(test_parser_parses_println_with_dot_separator)
    // RUN(test_parser_parses_println_with_comma_separator)
    // RUN(test_parser_parses_println_with_multiple_arguments)

    RUN(test_parser_function_definition);
END_MAIN
