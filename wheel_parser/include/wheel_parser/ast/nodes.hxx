#if !defined(NODES_HXX)
#define NODES_HXX

#include <cstdint>
#include <vector>
#include <wheel_lexer/token.hxx>
#include "wheel_parser/config.hxx"
#include "wheel_parser/ast/symbol.hxx"

using wheel_lexer::Token;

WHEEL_PARSER_AST_NAMESPACE
    enum class NodeKind : uint8_t {
        FunctionDeclaration,
        BlockStatement,
        LiteralExpression,
        IdentifierExpression,
        VariableDeclaration,
        ErrorStatement
    };

    struct Node {
        NodeKind node_kind;
        const Token *token;

        protected:
           explicit Node (NodeKind node_kind, 
                        const Token *token) noexcept;
    };

    struct StatementNode : Node {
        protected:
            explicit StatementNode(NodeKind node_kind, 
                                const Token *token) noexcept;
    };

    struct FunctionDeclaration final : StatementNode {
        SymbolID func_name;
        std::vector<SymbolID> func_params;
        StatementNode *func_body;

        explicit FunctionDeclaration(const Token *token, 
                                    SymbolID func_name,
                                    std::vector<SymbolID> func_params,
                                    StatementNode *func_body) noexcept;
    };

    struct BlockStatement final : StatementNode {
        std::vector<StatementNode*> statements;

        explicit BlockStatement(const Token *token,
                                std::vector<StatementNode*> statements) noexcept;
    };

    struct ExpressionNode : Node {
        protected:
            explicit ExpressionNode(NodeKind node_kind, const Token *token) noexcept;
    };

    struct LiteralExpression final : ExpressionNode {
        explicit LiteralExpression(const Token *token) noexcept;
    };

    struct IdentifierExpression final : ExpressionNode {
        SymbolID identifier_id;

        explicit IdentifierExpression(const Token *token, SymbolID identifier_id) noexcept;
    };

    struct VariableDeclaration final : StatementNode {
        SymbolID var_type;
        SymbolID var_name;
        ExpressionNode *initializer;

        explicit VariableDeclaration(const Token *token,
                                    SymbolID var_type, SymbolID var_name,
                                    ExpressionNode *initializer) noexcept;
    };

    struct ErrorStatement final : StatementNode {
        explicit ErrorStatement(const Token *token) noexcept;
    };

WHEEL_PARSER_END_NAMESPACE

#endif // NODES_HXX