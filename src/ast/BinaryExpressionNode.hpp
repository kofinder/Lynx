#ifndef LYNX_BINARY_OPERATION_NODE_HPP
#define LYNX_BINARY_OPERATION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>

using namespace LynxConstants;

namespace LynxAst {
    
    class BinaryExpressionNode: public Node {

        private:
            std::unique_ptr<Node> leftOperand;

            std::unique_ptr<Node> rightOperand;
            
            OperatorType operatorType;

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

            NodeType getNodeType() override { return NodeType::BINARY_OPERATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> ast_context);

            llvm::Value* generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> ast_context);

            llvm::Value* generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> ast_context);

            llvm::Value* generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> ast_context);

            llvm::Value* generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> ast_context);

            ~BinaryExpressionNode() override = default;
    };
}

#endif
