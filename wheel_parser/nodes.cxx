#include <vector>
#include "wheel_parser/ast/nodes.hxx"
#include "wheel_parser/ast/symbol.hxx"
#include <wheel_utils/logging.hxx>

USE_WHEEL_ASSERTION

using wheel_parser::ast::Node;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::FunctionDeclaration;
using wheel_parser::ast::BlockStatement;
using wheel_parser::ast::NodeKind;
using wheel_parser::ast::ExpressionNode;
using wheel_parser::ast::LiteralExpression;
using wheel_parser::ast::IdentifierExpression;
using wheel_parser::ast::VariableDeclaration;
using wheel_parser::ast::ErrorStatement;

Node::Node(NodeKind node_kind, 
        const Token* token) noexcept : node_kind(node_kind), token(token) {
        
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

StatementNode::StatementNode(NodeKind node_kind,
                             const Token *token) noexcept : Node(node_kind, token) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

FunctionDeclaration::FunctionDeclaration(const Token *token, 
                                        SymbolID func_name,
                                        std::vector<SymbolID> func_params,
                                        StatementNode *func_body) noexcept : 
            StatementNode(NodeKind::FunctionDeclaration, token), 
            func_name(func_name), func_params(std::move(func_params)), func_body(func_body) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

BlockStatement::BlockStatement(const Token *token, 
                        std::vector<StatementNode*> statements) noexcept : 
                        StatementNode(NodeKind::BlockStatement, token), 
                        statements(std::move(statements)) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

ExpressionNode::ExpressionNode(NodeKind node_kind, 
                        const Token *token) noexcept : Node(node_kind, token) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

LiteralExpression::LiteralExpression(const Token *token) noexcept : 
                        ExpressionNode(NodeKind::LiteralExpression, token) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

IdentifierExpression::IdentifierExpression(const Token *token,
                                        SymbolID identifier_id) noexcept :
                        ExpressionNode(NodeKind::IdentifierExpression, token),
                        identifier_id(identifier_id) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}


VariableDeclaration::VariableDeclaration(const Token *token,
                                        SymbolID var_type, SymbolID var_name,
                                        ExpressionNode *initializer) noexcept :
                        StatementNode(NodeKind::VariableDeclaration, token),
                        var_type(var_type), var_name(var_name),
                        initializer(initializer) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}

ErrorStatement::ErrorStatement(const Token *token) noexcept :
        StatementNode(NodeKind::ErrorStatement, token) {
        #if defined (WHEEL_ASSERTION)
                not_null<const Token*>(token);
        #endif 
}