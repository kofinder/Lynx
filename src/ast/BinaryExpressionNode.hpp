/**
 * @file BinaryExpressionNode.hpp
 * @brief Declares the BinaryExpressionNode class representing binary operations in the Lynx AST.
 * 
 * The BinaryExpressionNode class handles binary operations between expressions, supporting
 * integer, double, boolean, char, and string types. It provides LLVM IR code generation
 * specific to the operand types and supports cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores left and right operands for binary operations.
 * - Stores the operator type (e.g., +, -, *, /, etc.).
 * - Generates LLVM IR code for type-specific operations.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - Expression evaluation nodes in the compiler.
 * - Code generation routines for binary operations.
 * 
 * @see Node, OperatorType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_BINARY_OPERATION_NODE_HPP
#define LYNX_BINARY_OPERATION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class BinaryExpressionNode : public Node {

        private:

            std::unique_ptr<Node> leftOperand;

            std::unique_ptr<Node> rightOperand;
            
            OperatorType operatorType;
            
        private:

            llvm::Value* generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

            llvm::Value* generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

            llvm::Value* generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

            llvm::Value* generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

            llvm::Value* generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

            llvm::Value* generateIntegerWithOverflow(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext);

        public:

            explicit BinaryExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> rightNode
            ) : operatorType(oprType), rightOperand(std::move(rightNode)) {}

            explicit BinaryExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> leftNode, 
                std::unique_ptr<Node> rightNode
            ): operatorType(oprType), leftOperand(std::move(leftNode)), rightOperand(std::move(rightNode)) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::BINARY_OPERATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~BinaryExpressionNode() override = default;
    };
}

#endif
