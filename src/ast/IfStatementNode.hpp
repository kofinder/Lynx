/**
 * @file IfStatementNode.hpp
 * @brief Declares the IfStatementNode class, representing conditional "if" statements in the Lynx AST.
 * 
 * The IfStatementNode class models conditional branching with if, else-if, and optional else blocks.
 * It stores conditions and their corresponding statements and supports code generation via LLVM IR.
 * 
 * **Key Responsibilities:**
 * - Stores multiple conditional branches with associated statement blocks.
 * - Supports else branch as the default fallback.
 * - Generates LLVM IR for conditional branching.
 * - Provides deep cloning of the node and its branches.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for control flow.
 * 
 * @see DataType, LValueType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_IF_STATEMENT_NODE_HPP
#define LYNX_IF_STATEMENT_NODE_HPP

#include "Node.hpp" 
#include <constants/DataType.hpp> 
#include <constants/LValueType.hpp> 

namespace LynxAst {

    using namespace LynxConstants;

    class IfStatementNode : public Node {
        
        private:

            std::vector<std::unique_ptr<Node>> statements;

            std::vector<std::unique_ptr<Node>> conditions; 

            llvm::Value* generateIfElseIf(const AstContext& astContext);

        public:

            IfStatementNode() = default; 

            IfStatementNode(std::unique_ptr<Node> condNode, std::unique_ptr<Node> stmtNode) {
                conditions.push_back(std::move(condNode));
                statements.push_back(std::move(stmtNode));
            }
        
            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::IF_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            void addBranch(std::unique_ptr<Node> condNode, std::unique_ptr<Node> stmtNode);
            
            void addBranch(std::unique_ptr<IfStatementNode> ifNode);

            void addElseBranch(std::unique_ptr<Node> statementNode);

            ~IfStatementNode() override = default;
    };

}

#endif
