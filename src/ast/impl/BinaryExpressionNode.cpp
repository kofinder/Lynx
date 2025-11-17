#include <logger/Logger.hpp>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "BinaryExpressionNode.hpp"
#include <types/tmpl/TypeNumericPromotion.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace LynxTypes::TypePromotion;
    using namespace MetadataTypeConstants;

    llvm::Value* BinaryExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation ......");

        if (!leftOperand || !rightOperand) return nullptr;

        auto* lhsValue = leftOperand->generateCode(astContext->createContext());
        auto* rhsValue = rightOperand->generateCode(astContext->createContext());
        if (!lhsValue || !rhsValue) return nullptr;

        auto lhsBaseType = astContext->findType(lhsValue);
        auto rhsBaseType = astContext->findType(rhsValue);
        if (!lhsBaseType || !rhsBaseType) return nullptr;

        auto* lhsType = lhsBaseType->getLLVMType();
        auto* rhsType = rhsBaseType->getLLVMType();

        llvm::errs() << "LHS Type: "; lhsType->print(llvm::outs()); llvm::errs() << "\n";
        llvm::errs() << "RHS Type: "; rhsType->print(llvm::outs()); llvm::errs() << "\n";

        if (isNumericType(lhsType) && isNumericType(rhsType)) {
            auto& builder = astContext->getBuilder();
            auto promoted = promoteNumericOperands(lhsValue, rhsValue, builder);
            promoted.lhs = matchConstantType(builder, promoted.lhs, promoted.commonType);
            promoted.rhs = matchConstantType(builder, promoted.rhs, promoted.commonType);
            if (promoted.isFloating) {
                return generateDoubleCode(promoted.lhs, promoted.rhs, *astContext);
            } else {
                return generateIntegerCode(promoted.lhs, promoted.rhs, *astContext);
            }
        }

        if (TypeChecker::is<BooleanType>(lhsType)) return generateBooleanCode(lhsValue, rhsValue, *astContext);

        if (TypeChecker::is<StringType>(lhsType)) return generateStringCode(lhsValue, rhsValue, *astContext);

        if (TypeChecker::is<CharType>(lhsType)) return generateCharCode(lhsValue, rhsValue, *astContext);

        LOG_ERROR("Unsupported type in BinaryOperationNode.");
        return nullptr;
    }

    llvm::Value* BinaryExpressionNode::generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) {
        LOG_WARN("IR Code Generation ......");
        const auto resolveBinaryOperator = [this]() noexcept -> std::optional<std::pair<llvm::Instruction::BinaryOps, const char*>> {
            switch (operatorType) {
                case PLUS:        return std::make_pair(llvm::Instruction::Add, OPR_ADD);
                case MINUS:       return std::make_pair(llvm::Instruction::Sub, OPR_SUB);
                case MUL:         return std::make_pair(llvm::Instruction::Mul, OPR_MUL);
                case DIV:         return std::make_pair(llvm::Instruction::SDiv, OPR_DIV);
                case BITWISE_XOR: return std::make_pair(llvm::Instruction::Xor, OPR_XOR);
                default:          return std::nullopt;
            }    
        };
        
        const auto maybeInstr = resolveBinaryOperator();
        if (!maybeInstr) return nullptr;
            
        auto [instr, label] = *maybeInstr;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::BinaryOperator::Create(
            instr, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }

    llvm::Value* BinaryExpressionNode::generateIntegerWithOverflow(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) {
        auto& builder = astContext.getBuilder();
        auto* module = builder.GetInsertBlock()->getModule();
        llvm::Type* type = lhsValue->getType();
    
        llvm::Intrinsic::ID intrinsicID;
        switch (operatorType) {
            case PLUS: intrinsicID = llvm::Intrinsic::sadd_with_overflow; break;
            case MINUS: intrinsicID = llvm::Intrinsic::ssub_with_overflow; break;
            case MUL: intrinsicID = llvm::Intrinsic::smul_with_overflow; break;
            default: return nullptr; // Overflow intrinsic not supported
        }
    
        llvm::Function* intrinsic = llvm::Intrinsic::getOrInsertDeclaration(module, intrinsicID, {type});
        llvm::Value* resultStruct = builder.CreateCall(intrinsic, {lhsValue, rhsValue});
    
        // Extract the sum and overflow flag
        llvm::Value* sum = builder.CreateExtractValue(resultStruct, 0, "sum");
        llvm::Value* overflow = builder.CreateExtractValue(resultStruct, 1, "overflow");

        return sum;
    }


    llvm::Value* BinaryExpressionNode::generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) {
        LOG_WARN("IR Code Generation ......");
        const auto resolveBinaryOperator = [this]() noexcept -> std::optional<std::pair<llvm::Instruction::BinaryOps, const char*>> {
            switch (operatorType) {
                case PLUS:        return std::make_pair(llvm::Instruction::FAdd, OPR_ADD);
                case MINUS:       return std::make_pair(llvm::Instruction::FSub, OPR_SUB);
                case MUL:         return std::make_pair(llvm::Instruction::FMul, OPR_MUL);
                case DIV:         return std::make_pair(llvm::Instruction::FDiv, OPR_DIV);
                default:          return std::nullopt;
            }   
        };

        auto maybeInstr = resolveBinaryOperator();
        if (!maybeInstr) return nullptr;
            
        auto [instr, label] = *maybeInstr;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        auto* binOp =  llvm::BinaryOperator::Create(
            instr, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );

        llvm::FastMathFlags FMF;
        FMF.setFast();  // sets reassoc + contract + other safe optimizations
        binOp->setFastMathFlags(FMF);
        
        return binOp;
    }

    llvm::Value* BinaryExpressionNode::generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) {
        LOG_WARN("IR Code Generation ......");
        const auto resolveBinaryOperator = [this]() noexcept -> std::optional<std::pair<llvm::Instruction::BinaryOps, const char*>> {
            switch (operatorType) {
                case LOGICAL_AND:   return std::make_pair(llvm::Instruction::And, OPR_AND);
                case LOGICAL_OR:    return std::make_pair(llvm::Instruction::Or, OPR_OR);
                default:            return std::nullopt;
            }   
        };

        const auto maybeInstr = resolveBinaryOperator();
        if (!maybeInstr) return nullptr;
            
        auto [instr, label] = *maybeInstr;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::BinaryOperator::Create(
            instr, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }

    llvm::Value* BinaryExpressionNode::generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) {
        LOG_WARN("IR Code Generation ......");
        const auto resolveBinaryOperator = [this]() noexcept -> std::optional<std::pair<llvm::Instruction::BinaryOps, const char*>> {
            switch (operatorType) {
                case PLUS:        return std::make_pair(llvm::Instruction::Add, OPR_ADD);
                case MINUS:       return std::make_pair(llvm::Instruction::Sub, OPR_SUB);
                case MUL:         return std::make_pair(llvm::Instruction::Mul, OPR_MUL);
                case DIV:         return std::make_pair(llvm::Instruction::SDiv, OPR_DIV);
                default:          return std::nullopt;
            }   
        };

        const auto maybeInstr = resolveBinaryOperator();
        if (!maybeInstr) return nullptr;
            
        auto [instr, label] = *maybeInstr;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::BinaryOperator::Create(
            instr, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }

    llvm::Value* BinaryExpressionNode::generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext){
        if (operatorType == PLUS) {
            auto& builder = astContext.getBuilder();
            llvm::Function* function = builder.GetInsertBlock()->getParent();
            llvm::Value* concat = builder.CreateCall(function->getParent()->getFunction("string_concatenate"), {lhsValue, rhsValue});
            return concat;
        }

        LOG_ERROR("Unsupported operator for string type in BinaryOperationNode.");
        return nullptr;
    }

    std::unique_ptr<Node> BinaryExpressionNode::clone() const {
        return std::make_unique<BinaryExpressionNode>(
            operatorType,
            leftOperand->clone(),
            rightOperand->clone()
        ); 
    }

}