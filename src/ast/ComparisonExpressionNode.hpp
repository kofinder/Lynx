#ifndef LYNX_COMPARISION_EXPRESSION_NODE_HPP
#define LYNX_COMPARISION_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class ComparisonExpressionNode: public Node {
        
        private:

            OperatorType operatorType;

            std::unique_ptr<Node> leftOperand;

            std::unique_ptr<Node> rightOperand;

            llvm::Value* generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

            llvm::Value* generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

            llvm::Value* generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

            llvm::Value* generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

            llvm::Value* generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

            llvm::Value* generateEnumCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext);

        public:

            ComparisonExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> rightNode
            ): operatorType(oprType), rightOperand(std::move(rightNode)) {}

            ComparisonExpressionNode(
                OperatorType oprType, 
                std::unique_ptr<Node> 
                leftNode, std::unique_ptr<Node> rightNode
            ): operatorType(oprType), leftOperand(std::move(leftNode)), rightOperand(std::move(rightNode)) {}

            std::unique_ptr<Node> clone() const override;
            
            NodeType getNodeType() override { return NodeType::COMPARE_OPERATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ComparisonExpressionNode() override = default;
    };
}

#endif
