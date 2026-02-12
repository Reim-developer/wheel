#include <vector>
#include "wheel_parser/ast/nodes.hxx"

using std::move;
using wheel_parser::ast::Node;
using wheel_parser::ast::StatementNode;
using wheel_parser::ast::FunctionDeclaration;
using wheel_parser::ast::BlockStatement;
using wheel_parser::ast::NodeKind;

Node::Node(NodeKind node_kind, 
        const Token* token) noexcept : node_kind(node_kind), token(token) {}

StatementNode::StatementNode(NodeKind node_kind,
                             const Token *token) noexcept : Node(node_kind, token) {}

FunctionDeclaration::FunctionDeclaration(const Token *token, 
                                        SymbolID func_name,
                                        std::vector<SymbolID> func_params,
                                        StatementNode *func_body) noexcept : 
            StatementNode(NodeKind::FunctionDeclaration, token), 
            func_name(func_name), func_params(move(func_params)), func_body(func_body) {}

BlockStatement::BlockStatement(const Token *token, 
                            std::vector<StatementNode*> statements) noexcept : 
                            StatementNode(NodeKind::BlockStatement, token), 
                            statements(move(statements)){}
