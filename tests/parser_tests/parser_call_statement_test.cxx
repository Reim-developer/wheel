#include <__internal/__assert.hxx>

#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>

using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::CallExpression;
using wheel_parser::ast::ExpressionStatement;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::StringInterner;

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
    assert_eq(call->arguments[0]->token->str, "\"{}\"");
    assert_eq(call->arguments[1]->token->str, "name");
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
    assert_eq(call->arguments[0]->token->str, "\"{} {} {}\"");
    assert_eq(call->arguments[1]->token->str, "first");
    assert_eq(call->arguments[2]->token->str, "second");
    assert_eq(call->arguments[3]->token->str, "third");
DONE

TEST_MAIN
    RUN(test_parser_parses_println_with_dot_separator)
    RUN(test_parser_parses_println_with_comma_separator)
    RUN(test_parser_parses_println_with_multiple_arguments)
END_MAIN
