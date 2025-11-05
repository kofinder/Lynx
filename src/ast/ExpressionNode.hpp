/**
 * @file ExpressionNode.hpp
 * @brief Declares the ExpressionNode class, representing expressions in the Lynx AST.
 * 
 * The ExpressionNode class models a general expression, encapsulating the underlying operand
 * and the expression type. It supports cloning, type identification, and LLVM IR code generation.
 * 
 * **Key Responsibilities:**
 * - Encapsulates an expression node and its type.
 * - Supports LLVM IR generation for the expression.
 * - Enables deep cloning for AST transformations.
 * 
 * **Used By:**
 * - AST construction and semantic analysis.
 * - LLVM IR generation for expressions.
 * 
 * @see Node, ExpressionType, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_EXPRESSION_NODE_HPP
#define LYNX_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/Parameter.hpp>
#include <constants/expressions/ExpressionType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class ExpressionNode: public Node {

        public:

            std::unique_ptr<Node> expressionNode;
            
            ExpressionType expressionType;  

        public:

            explicit ExpressionNode(
                ExpressionType exprType, 
                std::unique_ptr<Node> operand
            ): expressionNode(std::move(operand)), expressionType(exprType) {}

            std::unique_ptr<Node> clone() const override;
            
            NodeType getNodeType() override { return NodeType::EXPRESSION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ExpressionNode() override = default;
    };
}

#endif