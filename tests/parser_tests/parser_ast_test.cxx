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

TEST(test_ast_block_statement_creation)
    Arena arena;

    BlockStatement *stmt1 = arena.allocate<BlockStatement>(
        nullptr, std::vector<StatementNode*>{}
    );
    BlockStatement *stmt2 = arena.allocate<BlockStatement>(
        nullptr, std::vector<StatementNode*>{}
    );
    BlockStatement *block_stmt = arena.allocate<BlockStatement>(
        nullptr, std::vector<StatementNode*>{stmt1, stmt2}
    );
    
    assert_eq(block_stmt->node_kind, NodeKind::BlockStatement);
    assert_eq(block_stmt->statements.size(), 2);
    assert_eq(block_stmt->statements[0], stmt1);
    assert_eq(block_stmt->statements[1], stmt2);
DONE 

TEST(test_ast_nested_blocks)
    Arena arena;
    StringInterner interner;

    auto func_name = interner.intern("nested_func");
    auto param     = interner.intern("param");

    BlockStatement *block1 = arena.allocate<BlockStatement>(
        nullptr,
        std::vector<StatementNode*>{}
    );
    BlockStatement *block2 = arena.allocate<BlockStatement>(
        nullptr,
        std::vector<StatementNode*>{block1}
    );

    FunctionDeclaration *func = arena.allocate<FunctionDeclaration>(
        nullptr, 
        func_name,
        std::vector<SymbolID>{param},
        block2
    );

    assert_eq(func->func_name, func_name);
    assert_eq(func->func_params.size(), 1);
    assert_eq(func->func_params[0], param);

    BlockStatement *func_body = static_cast<BlockStatement*>(func->func_body);
    assert_eq(func_body->node_kind, NodeKind::BlockStatement);
    assert_eq(func_body->statements.size(), 1);
    assert_eq(func_body->token, nullptr);
DONE 

TEST_MAIN
    RUN(test_ast_function_declaration_creation)
    RUN(test_ast_block_statement_creation)
    RUN(test_ast_nested_blocks)
END_MAIN