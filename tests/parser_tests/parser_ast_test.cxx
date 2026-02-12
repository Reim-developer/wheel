#include <string>
#include <format>
#include <wheel_parser/ast/nodes.hxx>
#include <wheel_parser/ast/symbol.hxx>
#include <wheel_parser/allocator.hxx>
#include <__internal/__assert.hxx>

using std::string;
using wheel_parser::ast::StringInterner;
using wheel_parser::ast::Node;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::BlockStatement;
using wheel_parser::ast::FunctionDeclaration;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::SymbolID;
using wheel_parser::Arena;

TEST(test_ast_function_declaration_creation)
    Arena arena;
    StringInterner interner;

    auto func_name = interner.intern("my_function");
    auto params    = std::vector<SymbolID>{
        interner.intern("param_1"),
        interner.intern("param_2")
    };
    
    BlockStatement *func_body = arena.allocate<BlockStatement>(
        nullptr, std::vector<StatementNode*>{}
    );
    FunctionDeclaration *func = arena.allocate<FunctionDeclaration>(
        nullptr,
        func_name,
        params,
        func_body
    );

   
    assert_eq(NodeKind::FunctionDeclaration, func->node_kind);
    assert_eq(func->func_params, params);
    assert_eq(func->func_name, func_name);
    assert_eq(func->func_body, func_body);
    assert_eq(func->token, nullptr);
DONE 

TEST_MAIN
    RUN(test_ast_function_declaration_creation)
END_MAIN