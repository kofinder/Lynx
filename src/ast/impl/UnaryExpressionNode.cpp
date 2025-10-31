#include "UnaryExpressionNode.hpp"
#include "VariableDeclarationNode.hpp"
#include <logger/Logger.hpp>
#include <types/builtins/ByteType.hpp>
#include <types/builtins/CharType.hpp>
#include <types/builtins/BooleanType.hpp>
#include <types/builtins/ShortType.hpp>
#include <types/builtins/IntegerType.hpp>
#include <types/builtins/LongType.hpp>
#include <types/builtins/StringType.hpp>
#include <types/builtins/DoubleType.hpp>
#include <types/builtins/FloatType.hpp>
#include <context/GlobalSymbolContext.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxTypes;


namespace LynxAst {

    template <typename T>
    bool isType(BaseType* type) {
        return dynamic_cast<T*>(type) != nullptr;
    }

    llvm::Value* UnaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("Generating code for UnaryExpressionNode...");

        auto& builder = astContext->getBuilder();
        llvm::Value* valueInstance = nullptr;
        llvm::Type* loadType = nullptr;
        llvm::Value* result = nullptr;
        llvm::Value* llvmVarRef = nullptr;

        if (this->expressionNode) {
            // If expressionNode exists, generate code for operand
            valueInstance = this->expressionNode->generateCode(astContext);
            loadType = valueInstance->getType();
        } else {
           auto astScope = astContext->getGlobalContext();
            Node* vNode = astScope->findVariable(this->variableName);
            if (!vNode) {
                LOG_ERROR("Variable '{}' is not defined in this scope.", this->variableName);
                throw std::runtime_error("Undefined variable: " + this->variableName);
            }

            // Ensure it's a VariableDeclarationNode
            auto* variable = dynamic_cast<VariableDeclarationNode*>(vNode);
            if (!variable) {
                LOG_ERROR("Variable '{}' is not of type VariableDeclarationNode.", this->variableName);
                throw std::runtime_error("Invalid variable type for unary operation.");
            }

            llvmVarRef = variable->getLLVMVariableRef();
            if (!llvmVarRef || !llvmVarRef->getType()->isPointerTy()) {
                LOG_ERROR("Variable '{}' is not a valid pointer.", this->variableName);
                throw std::runtime_error("Invalid LLVM variable reference.");
            }

            loadType = llvmVarRef->getType()->getPointerElementType();
            if (!loadType) {
                LOG_ERROR("Failed to determine pointer element type.");
                throw std::runtime_error("Failed to determine pointer element type.");
            }

            valueInstance = builder.CreateLoad(loadType, llvmVarRef, LynxLabelTypeConstants::lynxTempVariable);
        }

        switch (this->operatorType) {
            case PLUS:
                result = valueInstance;
                break;
            case MINUS:
                result = builder.CreateNeg(valueInstance);
                break;
            case INCREMENT:
                result = handleIncrementOrDecrement(astContext, valueInstance, loadType, llvmVarRef, true);
                break;
            case DECREMENT:
                result = handleIncrementOrDecrement(astContext, valueInstance, loadType, llvmVarRef, false);
                break;
            case LOGICAL_NOT:
                result = handleLogicalNot(astContext, valueInstance, loadType);
                break;
            default:
                LOG_ERROR("Unknown unary operator type");
                return nullptr;
        }

        return result;
    }


    llvm::Value* UnaryExpressionNode::handleIncrementOrDecrement(std::shared_ptr<AstContext> astContext, llvm::Value* valueInstance, llvm::Type* loadType, llvm::Value* llvmVarRef, bool isIncrement) {
        auto& builder = astContext->getBuilder();
        llvm::Value* result = nullptr;
        const char* label = isIncrement ? MetadataTypeConstants::oprIncTmp : MetadataTypeConstants::oprDecTmp;
        llvm::Value* incrementOrDecrementValue = builder.CreateAdd(valueInstance, llvm::ConstantInt::get(loadType, isIncrement ? 1 : -1), label);

        if (this->unaryExpressionType == UnaryExpressionType::POSTFIX) {
            result = valueInstance; // For postfix: return the value before the operation
            builder.CreateStore(incrementOrDecrementValue, llvmVarRef);
        } else {
            result = incrementOrDecrementValue; // For prefix: return the incremented/decremented value
            builder.CreateStore(result, llvmVarRef);
        }

        return result;
    }

    llvm::Value* UnaryExpressionNode::handleLogicalNot(std::shared_ptr<AstContext> astContext, llvm::Value* valueInstance, llvm::Type* loadType) {
        if (!loadType->isIntegerTy()) {
            LOG_ERROR("Logical NOT can only be applied to integer types.");
            throw std::runtime_error("Invalid type for logical NOT operation.");
        }
        auto& builder = astContext->getBuilder();
        const char* label = MetadataTypeConstants::oprNotTmp;
        return builder.CreateICmpEQ(valueInstance, llvm::ConstantInt::get(loadType, 0), label);
    }


    std::unique_ptr<Node> UnaryExpressionNode::clone() const {
        if (expressionNode) {
            return std::make_unique<UnaryExpressionNode>(operatorType, unaryExpressionType, expressionNode->clone());
        }
        return std::make_unique<UnaryExpressionNode>(operatorType, unaryExpressionType, variableName);
    }


}