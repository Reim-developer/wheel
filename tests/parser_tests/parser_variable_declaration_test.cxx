#include <__internal/__assert.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>

using wheel_parser::WheelParser;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::StringInterner;
using wheel_memory::Arena;
using wheel_lexer::Lexer;

TEST(test_normal_variable_declaration)
    Arena arena;
    Lexer lexer("int x = 10");
    StringInterner interner;
    WheelParser wheel_parser(lexer, arena, interner);

    const auto node = wheel_parser.parse_variable_declaration();

    assert_eq(node->node_kind, NodeKind::VariableDeclaration);
    assert_eq(node->initializer->token->str, "10");
    assert_eq(node->var_name, interner.intern("x"));
    assert_eq(node->var_type, interner.intern("int"));
    assert_eq(node->token->str, "int");
    assert_eq(node->token->start, 0);
    assert_eq(node->token->end, 3);
    assert_eq(node->token->kind, TokenKind::IDENT);
DONE 

TEST_MAIN
    RUN(test_normal_variable_declaration)
END_MAIN