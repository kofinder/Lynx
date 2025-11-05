/**
 * @file UnaryExpressionNode.hpp
 * @brief Declares the UnaryExpressionNode class representing unary expressions in the AST.
 * 
 * UnaryExpressionNode models unary operations in the Lynx language, such as increment/decrement,
 * logical NOT, and other unary operators applied to variables or expressions.
 * 
 * **Key Responsibilities:**
 * - Represent unary operations in the AST.
 * - Store the operator type and target (variable name or expression node).
 * - Generate LLVM IR code for unary operations.
 * - Handle increment/decrement and logical NOT operations internally.
 * 
 * @author Ko Thein
 * @date November 4, 2024
*/

#ifndef LYNX_UNARY_EXPRESSION_NODE_HPP
#define LYNX_UNARY_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>
#include <constants/expressions/UnaryExpressionType.hpp>


namespace LynxAst {
    
    using namespace LynxConstants;

    class UnaryExpressionNode : public Node {

        private:

            std::string variableName;

            OperatorType operatorType; 

            std::unique_ptr<Node> expressionNode;
            
            UnaryExpressionType unaryExpressionType;

            llvm::Value* handleIncrementOrDecrement(const AstContext& astContext, llvm::Value* valueInstance, llvm::Type* loadType, llvm::Value* llvmVarRef, bool isIncrement);

            llvm::Value* handleLogicalNot(const AstContext& astContext, llvm::Value* valueInstance, llvm::Type* loadType);

        public:

            explicit UnaryExpressionNode(
                OperatorType oprType, 
                UnaryExpressionType unaryExprType,
                std::string varName
            ) : operatorType(oprType), unaryExpressionType(unaryExprType), variableName(std::move(varName)) {}

            explicit UnaryExpressionNode(
                OperatorType oprType, 
                UnaryExpressionType unaryExprType, 
                std::unique_ptr<Node> exprNode
            ) : operatorType(oprType), unaryExpressionType(unaryExprType), expressionNode(std::move(exprNode)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::UNARY_OPERATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~UnaryExpressionNode() override = default;
    };
}

#endif
