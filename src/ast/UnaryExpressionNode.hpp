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
