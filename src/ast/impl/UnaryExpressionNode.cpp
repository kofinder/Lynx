#include <logger/Logger.hpp>
#include "UnaryExpressionNode.hpp"
#include "utils/NumericPromotion.hpp"
#include "utils/VariableUtils.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace MetadataTypeConstants;
    using namespace VariableUtils;

    llvm::Value* UnaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation ......");

        auto& builder = astContext->getBuilder();
        llvm::Value* valueInstance = nullptr;
        llvm::Type* valueType = nullptr;
        llvm::Value* llvmVarRef = nullptr;
        llvm::Value* result = nullptr;

        if (expressionNode) {
            valueInstance = expressionNode->generateCode(astContext);
            valueType = valueInstance->getType();
        } else {
            auto resolved = resolveVariable(*astContext, variableName);
            if (!resolved.value || !resolved.reference) {
                LOG_ERROR("Failed to resolve variable '{}'", variableName);
                throw std::runtime_error("Variable resolution failed.");
            }
            valueInstance = resolved.value;
            llvmVarRef   = resolved.reference;
            valueType    = valueInstance->getType();
        }

        switch (operatorType) {
            case PLUS:          result = valueInstance; break;
            case MINUS:         result = builder.CreateNeg(valueInstance); break;
            case INCREMENT:     result = handleIncrementOrDecrement(*astContext, valueInstance, valueType, llvmVarRef, true); break;
            case DECREMENT:     result = handleIncrementOrDecrement(*astContext, valueInstance, valueType, llvmVarRef, false); break;
            case LOGICAL_NOT:   result = handleLogicalNot(*astContext, valueInstance, valueType); break;
            case BITWISE_NOT:   result = builder.CreateNot(valueInstance, TMP_BIT_NOT); break;
            default:            throw std::runtime_error("Unknown unary operator type.");
        }

        return result;
    }


    llvm::Value* UnaryExpressionNode::handleIncrementOrDecrement(const AstContext& astContext, llvm::Value* valueInstance, llvm::Type* valueType, llvm::Value* llvmVarRef, bool isIncrement) {
        auto& builder = astContext.getBuilder();
        const char* label = isIncrement ? OPR_INC : OPR_DEC;
        llvm::Value* delta = llvm::ConstantInt::get(valueType, isIncrement ? 1 : -1);
        llvm::Value* updatedValue = builder.CreateAdd(valueInstance, delta, label);

        if (unaryExpressionType == UnaryExpressionType::POSTFIX) {
            builder.CreateStore(updatedValue, llvmVarRef);
            return valueInstance;
        } else {
            builder.CreateStore(updatedValue, llvmVarRef);
            return updatedValue;
        }
    }

    llvm::Value* UnaryExpressionNode::handleLogicalNot(const AstContext& astContext, llvm::Value* valueInstance, llvm::Type* loadType) {
        if (!loadType->isIntegerTy()) {
            LOG_ERROR("Logical NOT ('!') can only be applied to integer or boolean types.");
            throw std::runtime_error("Invalid operand type for logical NOT.");
        }

        auto& builder = astContext.getBuilder();
        return builder.CreateICmpEQ(valueInstance, llvm::ConstantInt::get(loadType, 0), OPR_NOT);
    }

    std::unique_ptr<Node> UnaryExpressionNode::clone() const {
        if (expressionNode) {
            return std::make_unique<UnaryExpressionNode>(operatorType, unaryExpressionType, expressionNode->clone());
        }
        return std::make_unique<UnaryExpressionNode>(operatorType, unaryExpressionType, variableName);
    }

}