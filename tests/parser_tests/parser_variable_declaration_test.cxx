#include <__internal/__assert.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_parser/error.hxx>
#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>

using wheel_parser::WheelParser;
using wheel_parser::ParseErrorCode;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::StringInterner;
using wheel_parser::ast::VariableDeclaration;
using wheel_memory::Arena;
using wheel_lexer::Lexer;

TEST(test_normal_variable_declaration)
    Arena arena;
    Lexer lexer("var x: int = 10");
    StringInterner interner;
    WheelParser wheel_parser(lexer, arena, interner);

    const auto node = wheel_parser.parse_variable_declaration();
    const auto variable = static_cast<VariableDeclaration*>(node);

    assert_eq(node->node_kind, NodeKind::VariableDeclaration);
    assert_eq(variable->initializer->token->str, "10");
    assert_eq(variable->var_name, interner.intern("x"));
    assert_eq(variable->var_type, interner.intern("int"));
    assert_eq(node->token->str, "var");
    assert_eq(node->token->start, 0);
    assert_eq(node->token->end, 3);
    assert_eq(node->token->kind, TokenKind::IDENT);
    assert_eq(wheel_parser.error_count(), 0);
DONE

TEST(test_variable_declaration_error_node)
    Arena arena;
    Lexer lexer("var x int = 10");
    StringInterner interner;
    WheelParser wheel_parser(lexer, arena, interner);

    const auto node = wheel_parser.parse_variable_declaration();
    const auto errors = wheel_parser.errors_data();
    const auto diagnostics = wheel_parser.collect_diagnostics("example.wl");

    assert_eq(node->node_kind, NodeKind::ErrorStatement);
    assert_eq(wheel_parser.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(ParseErrorCode::ExpectedColon));
    assert_eq(errors[0].location.line, 1);
    assert_eq(errors[0].location.column, 7);
    assert_eq(diagnostics.size(), 1);
    assert_eq(diagnostics.data()[0].source_line, "var x int = 10");
    assert_eq(diagnostics.data()[0].marker, "      ^^^");
    assert_eq(diagnostics.data()[0].full_message.empty(), false);
DONE

TEST(test_variable_declaration_missing_type_keyword)
    Arena arena;
    Lexer lexer("var x: foo = 10");
    StringInterner interner;
    WheelParser wheel_parser(lexer, arena, interner);

    const auto node = wheel_parser.parse_variable_declaration();
    const auto errors = wheel_parser.errors_data();

    assert_eq(node->node_kind, NodeKind::ErrorStatement);
    assert_eq(wheel_parser.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(ParseErrorCode::ExpectedTypeKeyword));
DONE

TEST_MAIN
    RUN(test_normal_variable_declaration)
    RUN(test_variable_declaration_error_node)
    RUN(test_variable_declaration_missing_type_keyword)
END_MAIN
