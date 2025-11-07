/**
 * @file PrimaryExpressionNode.hpp
 * @brief Declares the PrimaryExpressionNode class, representing primary expressions in the Lynx AST.
 * 
 * The PrimaryExpressionNode class models basic expressions such as literals, identifiers, or other
 * primary expression constructs. It holds the expression type and an inner expression node, providing
 * AST traversal, cloning, and LLVM IR code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the primary expression type and the inner expression node.
 * - Supports LLVM IR code generation for primary expressions.
 * - Provides deep cloning of the node and its inner expression.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for expression evaluation.
 * 
 * @see IdentifierNode, ExpressionNode, PrimaryExpressionType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_PRIMARY_EXPRESSION_NODE_HPP
#define LYNX_PRIMARY_EXPRESSION_NODE_HPP

#include <string>
#include "Node.hpp"
#include "ExpressionNode.hpp"
#include "IdentifierNode.hpp"
#include <constants/expressions/PrimaryExpressionType.hpp>


namespace LynxAst {

    using namespace LynxContext;
    using namespace LynxConstants;

    class PrimaryExpressionNode : public Node {

        private:

            PrimaryExpressionType primaryExpType;

            std::unique_ptr<Node> innerExpNode;
    
        public:
    
            PrimaryExpressionNode(
                PrimaryExpressionType exprType, 
                std::unique_ptr<Node> exprNode
            ) : primaryExpType(exprType), innerExpNode(std::move(exprNode)) {}

            PrimaryExpressionNode(
                PrimaryExpressionType exprType, 
                std::unique_ptr<IdentifierNode> identNode
            ) : primaryExpType(exprType), innerExpNode(std::move(identNode)) {}
            
            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::PRIMARY_EXPR_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] Node* getInnerExpression() const { return innerExpNode.get(); }
    
            constexpr inline PrimaryExpressionType getPrimaryExpressionType() const { return primaryExpType; }

            ~PrimaryExpressionNode() override = default;
    };
}

#endif
