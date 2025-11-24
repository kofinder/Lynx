/**
 * @file ExpressionStatementNode.hpp
 * @brief Declares the ExpressionStatementNode class, representing expression statements in the Lynx AST.
 * 
 * The ExpressionStatementNode class models statements that are single expressions, such as
 * assignments, function calls, or arithmetic expressions. It provides cloning, LLVM IR code
 * generation, and encapsulates the underlying expression node.
 * 
 * **Key Responsibilities:**
 * - Stores a single expression as a statement.
 * - Generates LLVM IR for the expression.
 * - Provides deep cloning for AST transformations.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for expression statements.
 * 
 * @see Node, ExpressionNode, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_EXPRESSION_STATEMENT_NODE_HPP
#define LYNX_EXPRESSION_STATEMENT_NODE_HPP


#include "Node.hpp"
#include <constants/Parameter.hpp>
#include <constants/expressions/ExpressionType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class ExpressionStatementNode : public Node {

        protected:

            std::unique_ptr<Node> expressionNode; 

        public:

            ExpressionStatementNode(std::unique_ptr<Node> operand): expressionNode(std::move(operand)) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::EXPRESSION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ExpressionStatementNode() override = default;

    };
}

#endif
