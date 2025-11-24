/**
 * @file EnumValueExpressionNode.hpp
 * @brief Declares the EnumValueExpressionNode class for evaluating enum value expressions in the Lynx AST.
 * 
 * The EnumValueExpressionNode class represents expressions used to compute enum values, including
 * literal values and binary operations. It supports evaluation at compile-time and integrates
 * with the AST infrastructure for cloning and code generation.
 * 
 * **Key Responsibilities:**
 * - Represents integer values or binary operations for enum values.
 * - Supports evaluation of constant expressions at compile time.
 * - Provides deep cloning for AST transformations.
 * 
 * **Used By:**
 * - Enum declaration and initialization.
 * - AST semantic analysis and constant folding.
 * 
 * @see Node, OperatorType, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_ENUM_VALUE_EXPRESSION_NODE_HPP
#define LYNX_ENUM_VALUE_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class EnumValueExpressionNode : public Node {

        private:

            int value;

            std::unique_ptr<EnumValueExpressionNode> leftOperand;

            std::unique_ptr<EnumValueExpressionNode> rightOperand;
            
            OperatorType operatorType;


        public:

            explicit EnumValueExpressionNode(int val) : value(val), operatorType(OperatorType::NONE) {}

            explicit EnumValueExpressionNode(
                OperatorType oprType,
                std::unique_ptr<EnumValueExpressionNode> leftNode,
                std::unique_ptr<EnumValueExpressionNode> rightNode
            ): operatorType(oprType), leftOperand(std::move(leftNode)), rightOperand(std::move(rightNode)) {}

            std::unique_ptr<Node> clone() const override;

            int evaluate() const;
            
            inline constexpr NodeType getNodeType() override { return NodeType::ENUM_VALUE_EXPRESSION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return nullptr; }

            ~EnumValueExpressionNode() override = default;
    };

}

#endif
