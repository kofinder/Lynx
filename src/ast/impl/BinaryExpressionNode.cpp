#include "BinaryExpressionNode.hpp"
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

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxTypes;


namespace LynxAst {

    template <typename T>
    bool isType(BaseType* type) {
        return dynamic_cast<T*>(type) != nullptr;
    }

    
    llvm::Value* BinaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("BinaryOperationNode code generation ......");

        if (!leftOperand || !rightOperand) {
            LOG_ERROR("Null operand detected in BinaryOperationNode");
            return nullptr;
        }

        llvm::Value* lhsValue = leftOperand->generateCode(astContext->createContext());
        llvm::Value* rhsValue = rightOperand->generateCode(astContext->createContext());

        if (!lhsValue || !rhsValue) {
            LOG_ERROR("Failed to generate code for operands.");
            return nullptr;
        }

        auto rhsType = astContext->findType(rhsValue);
        BaseType* baseType = rhsType.get();

        if (!baseType) {
            LOG_ERROR("Unknown type detected in BinaryOperationNode.");
            return nullptr;
        }

        LOG_INFO("Detected BaseType: {}", typeid(*baseType).name());

        // Use helper function to check types
        if (isType<ShortType>(baseType) || isType<IntegerType>(baseType) || isType<LongType>(baseType) || isType<ByteType>(baseType)) {
            return generateIntegerCode(lhsValue, rhsValue, astContext);
        }
        if (isType<FloatType>(baseType) || isType<DoubleType>(baseType)) {
            return generateDoubleCode(lhsValue, rhsValue, astContext);
        }
        if (isType<BooleanType>(baseType)) {
            return generateBooleanCode(lhsValue, rhsValue, astContext);
        }
        if (isType<StringType>(baseType)) {
            return generateStringCode(lhsValue, rhsValue, astContext);
        }
        if (isType<CharType>(baseType)) {
            return generateCharCode(lhsValue, rhsValue, astContext);
        }
       

        LOG_ERROR("Unsupported type in BinaryOperationNode.");
        return nullptr;
    }

    llvm::Value* BinaryExpressionNode::generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> astContext) {
        LOG_WARN("BinaryOperationNode::generateIntegerCode code generation ......");
        llvm::Instruction::BinaryOps instr;
        auto& builder = astContext->getBuilder();        
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
        auto label = "binaryopr";

        switch (this->operatorType) {
            case PLUS:
                instr = llvm::Instruction::Add;
                label = MetadataTypeConstants::oprAddTmp;
                break;
            case MINUS:
                instr = llvm::Instruction::Sub;
                label = MetadataTypeConstants::oprSubTmp;
                break;
            case MUL:
                instr = llvm::Instruction::Mul;
                label = MetadataTypeConstants::oprMulTmp;
                break;
            case DIV:
                instr = llvm::Instruction::SDiv;
                label = MetadataTypeConstants::oprDiffTmp;
                break;
            case BITWISE_XOR:
                instr = llvm::Instruction::Xor;
                label = MetadataTypeConstants::oprXor;
                break;
            default:
                return nullptr;
        }

        if (lhsValue->getType() != rhsValue->getType()) {
            LOG_ERROR("Operand types do not match for Bitwise XOR operation");
            return nullptr;
        }

        return llvm::BinaryOperator::Create(instr, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }


    llvm::Value* BinaryExpressionNode::generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> astContext) {
        LOG_WARN("BinaryOperationNode::generateDoubleCode code generation ......");
        llvm::Instruction::BinaryOps instr;
        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
        auto label = "binaryopr";

        switch (this->operatorType) {
            case PLUS:
                instr = llvm::Instruction::FAdd;
                label = MetadataTypeConstants::oprAddTmp;
                break;
            case MINUS:
                instr = llvm::Instruction::FSub;
                label = MetadataTypeConstants::oprSubTmp;
                break;
            case MUL:
                instr = llvm::Instruction::FMul;
                label = MetadataTypeConstants::oprMulTmp;
                break;
            case DIV:
                instr = llvm::Instruction::FDiv;
                label = MetadataTypeConstants::oprDiffTmp;
                break;
            default:
                return nullptr;
        }

        if (lhsValue->getType() != rhsValue->getType()) {
            LOG_ERROR("Operand types do not match for Bitwise XOR operation");
            return nullptr;
        }

        return llvm::BinaryOperator::Create(instr, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* BinaryExpressionNode::generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> astContext) {
        LOG_WARN("BinaryOperationNode::generateBooleanCode code generation ......");
        llvm::Instruction::BinaryOps instr;
        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
        auto label = "binaryopr";

        switch (this->operatorType) {
            case LOGICAL_AND:
                instr = llvm::Instruction::And;
                label = MetadataTypeConstants::oprAndTmp;
                break;
            case LOGICAL_OR:
                instr = llvm::Instruction::Or;
                label = MetadataTypeConstants::oprOrTmp;
                break;
            default:
                return nullptr;
        }

        if (lhsValue->getType() != rhsValue->getType()) {
            LOG_ERROR("Operand types do not match for Bitwise XOR operation");
            return nullptr;
        }

        return llvm::BinaryOperator::Create(instr, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* BinaryExpressionNode::generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> astContext) {
        LOG_WARN("BinaryOperationNode::generateCharCode code generation ......");
        llvm::Instruction::BinaryOps instr;
        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
        auto label = "binaryopr";

        switch (this->operatorType) {
            case PLUS:
                instr = llvm::Instruction::Add;
                label = MetadataTypeConstants::oprAddTmp;
                break;
            case MINUS:
                instr = llvm::Instruction::Sub;
                label = MetadataTypeConstants::oprSubTmp;
                break;
            case MUL:
                instr = llvm::Instruction::Mul;
                label = MetadataTypeConstants::oprMulTmp;
                break;
            case DIV:
                instr = llvm::Instruction::SDiv;
                label = MetadataTypeConstants::oprDiffTmp;
                break;
            default:
                return nullptr;
        }

        if (lhsValue->getType() != rhsValue->getType()) {
            LOG_ERROR("Operand types do not match for Bitwise XOR operation");
            return nullptr;
        }

        return llvm::BinaryOperator::Create(instr, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* BinaryExpressionNode::generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, std::shared_ptr<AstContext> astContext){
        LOG_WARN("BinaryOperationNode::generateStringCode code generation ......");
        if (this->operatorType == PLUS) {
            // Concatenate the two strings (assuming StringType is being handled)
            // This assumes that `lhsValue` and `rhsValue` are string values
            auto& builder = astContext->getBuilder();
            llvm::BasicBlock* currentBlock = builder.GetInsertBlock();
            llvm::Function* function = builder.GetInsertBlock()->getParent();
            llvm::Value* concat = builder.CreateCall(function->getParent()->getFunction("string_concatenate"), {lhsValue, rhsValue});
            return concat;
        }

        LOG_ERROR("Unsupported operator for string type in BinaryOperationNode.");
        return nullptr;
    }

    std::unique_ptr<Node> BinaryExpressionNode::clone() const {
        return std::make_unique<BinaryExpressionNode>(
            this->operatorType,
            this->leftOperand->clone(),
            this->rightOperand->clone()
        ); 
    }

}