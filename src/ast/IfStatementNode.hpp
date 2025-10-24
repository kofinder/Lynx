#ifndef LYNX_IF_STATEMENT_NODE_HPP
#define LYNX_IF_STATEMENT_NODE_HPP

// Includes
#include "Node.hpp"                           // Base class for AST nodes
#include <constants/DataType.hpp>         // For variable type constants
#include <constants/LValueType.hpp>               // For L-value constants

using namespace LynxConstants;

namespace LynxAst {

    /**
     * @class IfStatementNode
     * @brief Represents an `if` or `if-else` statement in the Abstract Syntax Tree (AST).
     *
     * This class is used to model conditional statements (`if`, `if-else`) in the AST.
     * It supports generating LLVM IR for these constructs and managing branches and conditions.
     *
     * Key Responsibilities:
     * - Store conditions and corresponding statements.
     * - Generate LLVM IR for the if-else chain.
     * - Support adding new branches and else blocks.
     */
    class IfStatementNode: public Node {
        
        private:

            std::vector<std::unique_ptr<Node>> statements;

            std::vector<std::unique_ptr<Node>> conditions; 

        public:

            IfStatementNode() = default; // ✅ Allow default construction

            IfStatementNode(std::unique_ptr<Node> condNode, std::unique_ptr<Node> stmtNode) {
                conditions.push_back(std::move(condNode));
                statements.push_back(std::move(stmtNode));
            }
        
            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::IF_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* generateIfElseIf(std::shared_ptr<AstContext> astContext);

            void addBranch(std::unique_ptr<Node> condNode, std::unique_ptr<Node> stmtNode);
            
            void addBranch(std::unique_ptr<IfStatementNode> ifNode);

            void addElseBranch(std::unique_ptr<Node> statementNode);

            ~IfStatementNode() override = default;
    };

}

#endif
