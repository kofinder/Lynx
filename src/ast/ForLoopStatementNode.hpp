/**
 * @file ForLoopStatementNode.hpp
 * @brief Declares the ForLoopStatementNode class, representing for-loop constructs in the Lynx AST.
 * 
 * The ForLoopStatementNode class models a standard for-loop, including initialization, condition,
 * increment/decrement expressions, and loop body statements. It supports pre/post iteration nodes
 * and additional hooks for before/after execution logic. LLVM IR code generation is provided for
 * integration with the Lynx compiler backend.
 * 
 * **Key Responsibilities:**
 * - Stores loop components: condition, body, pre-loop and post-loop nodes.
 * - Supports hooks for before and after loop execution.
 * - Generates LLVM IR for the full loop construct.
 * - Provides deep cloning for AST transformations.
 * 
 * **Used By:**
 * - AST construction and semantic analysis.
 * - LLVM IR generation for loop constructs.
 * 
 * @see Node, ExpressionNode, StatementListNode, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FOR_LOOP_STATEMENT_NODE_HPP 
#define LYNX_FOR_LOOP_STATEMENT_NODE_HPP


#include "Node.hpp"
#include "StatementListNode.hpp"
#include "ExpressionNode.hpp"
#include <constants/Parameter.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class ForLoopStatementNode : public Node {

        private:

            std::unique_ptr<ExpressionNode> conditionNode;

            std::unique_ptr<StatementListNode> statementNode;
        
        public:

            std::unique_ptr<Node> preLoopNode;
            std::unique_ptr<Node> postLoopNode;
            std::unique_ptr<Node> beforeNode;
            std::unique_ptr<Node> afterNode;
        
            ForLoopStatementNode(
                std::unique_ptr<ExpressionNode> cond, 
                std::unique_ptr<StatementListNode> stmts
            ) : conditionNode(std::move(cond)), statementNode(std::move(stmts)) {}
        
            std::unique_ptr<Node> clone() const override;
            
            NodeType getNodeType() override { return NodeType::LOOP_NODE; }
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
        
            ~ForLoopStatementNode() override = default;
    };        
}

#endif
