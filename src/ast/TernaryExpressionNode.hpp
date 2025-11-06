/**
 * @file TernaryExpressionNode.hpp
 * @brief Declares the TernaryExpressionNode class representing ternary (?:) expressions in the AST.
 * 
 * TernaryExpressionNode models the conditional operator `condition ? then : else` in the Lynx language.
 * 
 * Responsibilities:
 * - Store the condition, then-expression, and else-expression nodes.
 * - Generate LLVM IR for the ternary expression.
 * - Support cloning for AST transformations and analyses.
 * 
 * @author Ko Thein
 * @date November 4, 2024
*/


#ifndef LYNX_TERNARY_OPERATOR_STATEMENT_HPP
#define LYNX_TERNARY_OPERATOR_STATEMENT_HPP


#include "Node.hpp" 

namespace LynxAst {

    using namespace LynxConstants;

    class TernaryExpressionNode: public Node {

        private:

            std::unique_ptr<Node> conditionExpression;

            std::unique_ptr<Node> thenExpression;

            std::unique_ptr<Node> endExpression;
            
        public:

            TernaryExpressionNode(
                std::unique_ptr<Node> conditionExpr, 
                std::unique_ptr<Node> thenExpr, 
                std::unique_ptr<Node> endExpr
            ): conditionExpression(std::move(conditionExpr)), thenExpression(std::move(thenExpr)), endExpression(std::move(endExpr)) {}

            inline constexpr NodeType getNodeType() override { return NodeType::TERNARY_OPERATOR_NODE; }

            std::unique_ptr<Node> clone() const override ;

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
            
            ~TernaryExpressionNode() override = default;
    };
}

#endif
