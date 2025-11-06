/**
 * @file ComparisonExpressionNode.hpp
 * @brief Declares the ComparisonExpressionNode class representing comparison operations in the Lynx AST.
 * 
 * The ComparisonExpressionNode class handles binary and unary comparison operations between expressions,
 * including type-specific code generation for integer, floating-point, boolean, char, string, and enum types.
 * 
 * **Key Responsibilities:**
 * - Stores left and right operands for comparison operations.
 * - Stores the comparison operator type (e.g., ==, !=, <, >).
 * - Generates LLVM IR code specific to the operand types.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - Expression evaluation nodes in the compiler.
 * - Code generation for comparison operations.
 * 
 * @see Node, OperatorType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_COMPARISION_EXPRESSION_NODE_HPP
#define LYNX_COMPARISION_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class ComparisonExpressionNode : public Node {
        
        private:

            OperatorType operatorType;

            std::unique_ptr<Node> leftOperand;

            std::unique_ptr<Node> rightOperand;

        private:

            llvm::Value* generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

            llvm::Value* generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

            llvm::Value* generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

            llvm::Value* generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

            llvm::Value* generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

            llvm::Value* generateEnumCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const;

        public:

            ComparisonExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> rightNode
            ) : operatorType(oprType), rightOperand(std::move(rightNode)) {}

            ComparisonExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> 
                leftNode, std::unique_ptr<Node> rightNode
            ) : operatorType(oprType), leftOperand(std::move(leftNode)), rightOperand(std::move(rightNode)) {}

            std::unique_ptr<Node> clone() const override;
            
            inline constexpr NodeType getNodeType() override { return NodeType::COMPARE_OPERATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ComparisonExpressionNode() override = default;
    };
}

#endif
