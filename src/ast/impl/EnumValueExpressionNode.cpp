#include "EnumValueExpressionNode.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;

namespace LynxAst {

    int EnumValueExpressionNode::evaluate() const {
        if (operatorType == OperatorType::NONE) {
            return value;
        }
    
        int lhs = leftOperand->evaluate();
        int rhs = rightOperand->evaluate();
    
        switch (operatorType) {
            case OperatorType::PLUS:    return lhs + rhs;
            case OperatorType::MINUS:   return lhs - rhs;
            case OperatorType::MUL:     return lhs * rhs;
            case OperatorType::DIV:     return rhs != 0 ? lhs / rhs : 0;
            case OperatorType::LSHIFT: return lhs << rhs;
            case OperatorType::RSHIFT: return lhs >> rhs;
            default:                    return 0;
        }
    }    

    std::unique_ptr<Node> EnumValueExpressionNode::clone() const {
        std::unique_ptr<EnumValueExpressionNode> clonedLeft;
        std::unique_ptr<EnumValueExpressionNode> clonedRight;
    
        if (leftOperand) {
            clonedLeft = std::unique_ptr<EnumValueExpressionNode>(static_cast<EnumValueExpressionNode*>(leftOperand->clone().release()));
        }
    
        if (rightOperand) {
            clonedRight = std::unique_ptr<EnumValueExpressionNode>(static_cast<EnumValueExpressionNode*>(rightOperand->clone().release()));
        }
    
        auto clonedNode = std::make_unique<EnumValueExpressionNode>(operatorType, std::move(clonedLeft), std::move(clonedRight));
    
        clonedNode->value = this->value;
    
        return clonedNode;
    }
}