#ifndef LYNX_ENUM_VALUE_EXPRESSION_NODE_HPP
#define LYNX_ENUM_VALUE_EXPRESSION_NODE_HPP

#include "Node.hpp"
#include <constants/OperatorType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class EnumValueExpressionNode: public Node {

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
            
            NodeType getNodeType() override { return NodeType::ENUM_VALUE_EXPRESSION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return nullptr; }

            ~EnumValueExpressionNode() override = default;
    };

}

#endif
