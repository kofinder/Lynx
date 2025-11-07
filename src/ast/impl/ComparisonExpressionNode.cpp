#include <logger/Logger.hpp>
#include <types/tmpl/TypeChecker.hpp>
#include "utils/NumericPromotion.hpp"
#include "ComparisonExpressionNode.hpp"

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace TypePromotion;
    using namespace MetadataTypeConstants;
    
    llvm::Value* ComparisonExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation ......");

        if (!leftOperand || !rightOperand) {
            LOG_ERROR("Null operand detected");
            return nullptr;
        }

        auto* lhsValue = leftOperand->generateCode(astContext->createContext());
        auto* rhsValue = rightOperand->generateCode(astContext->createContext());

        if (!lhsValue || !rhsValue) {
            LOG_ERROR("Failed to generate code for operands.");
            return llvm::ConstantInt::getFalse(astContext->getLLVMContext());
        }

        auto lhsBaseType = astContext->findType(lhsValue);
        auto rhsBaseType = astContext->findType(rhsValue);

        auto* lhsType = lhsBaseType->getLLVMType();
        auto* rhsType = rhsBaseType->getLLVMType();

        llvm::errs() << "[Comparison] LHS Type: "; lhsType->print(llvm::outs()); llvm::errs() << "\n";
        llvm::errs() << "[Comparison] RHS Type: "; rhsType->print(llvm::outs()); llvm::errs() << "\n";

        if (isNumericType(lhsType) && isNumericType(rhsType)) {
            auto promoted = promoteNumericOperands(lhsValue, rhsValue, astContext->getBuilder());
            if (promoted.isFloating) {
                return generateDoubleCode(promoted.lhs, promoted.rhs, *astContext);
            } else {
                return generateIntegerCode(promoted.lhs, promoted.rhs, *astContext);
            }
        }
    
        if (TypeChecker::is<BooleanType>(lhsType)) return generateBooleanCode(lhsValue, rhsValue, *astContext);
        if (TypeChecker::is<StringType>(lhsType)) return generateStringCode(lhsValue, rhsValue, *astContext);
        if (TypeChecker::is<CharType>(lhsType)) return generateCharCode(lhsValue, rhsValue, *astContext);
        if (TypeChecker::is<EnumType>(lhsType)) return generateEnumCode(lhsValue, rhsValue, *astContext);
       
        LOG_ERROR("Unsupported type in ComparisonExpressionNode.");
        return llvm::ConstantInt::getFalse(astContext->getLLVMContext());
    }

    llvm::Value* ComparisonExpressionNode::generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {        
        LOG_WARN("IR Code Generation ......");

        const bool isUnsigned = lhsValue->getType()->isIntegerTy() && !lhsValue->getType()->getScalarSizeInBits();
        auto resolveComparisonOperator = [this, isUnsigned]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>> {
            switch (operatorType) {
                case GREATER_THAN:          return std::make_pair(isUnsigned ? llvm::CmpInst::ICMP_UGT : llvm::CmpInst::ICMP_SGT, OPR_GT);
                case LESS_THAN:             return std::make_pair(isUnsigned ? llvm::CmpInst::ICMP_ULT : llvm::CmpInst::ICMP_SLT, OPR_LT);
                case GREATER_THAN_EQUAL:    return std::make_pair(isUnsigned ? llvm::CmpInst::ICMP_UGE : llvm::CmpInst::ICMP_SGE, OPR_GE);
                case LESS_THAN_EQUAL:       return std::make_pair(isUnsigned ? llvm::CmpInst::ICMP_ULE : llvm::CmpInst::ICMP_SLE, OPR_LE);
                case EQUALITY:              return std::make_pair(llvm::CmpInst::ICMP_EQ, OPR_EQ);
                case NOT_EQUALITY:          return std::make_pair(llvm::CmpInst::ICMP_NE, OPR_NE);
                default:                    return std::nullopt;
            }
        };

        const auto maybeComparison = resolveComparisonOperator();
        if (!maybeComparison) return nullptr;
    
        const auto& [predicate, label] = *maybeComparison;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::CmpInst::Create(
            llvm::Instruction::ICmp, 
            predicate, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }

    llvm::Value* ComparisonExpressionNode::generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {
        LOG_WARN("IR Code Generation ......");

        const auto resolveComparisonOperator = [this]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>> {
            switch (operatorType) {
                case GREATER_THAN:          return std::make_pair(llvm::CmpInst::FCMP_OGT, OPR_GT);
                case LESS_THAN:             return std::make_pair(llvm::CmpInst::FCMP_OLT, OPR_LT);
                case GREATER_THAN_EQUAL:    return std::make_pair(llvm::CmpInst::FCMP_OGE, OPR_GE);
                case LESS_THAN_EQUAL:       return std::make_pair(llvm::CmpInst::FCMP_OLE, OPR_LE);
                case EQUALITY:              return std::make_pair(llvm::CmpInst::FCMP_OEQ, OPR_EQ);
                case NOT_EQUALITY:          return std::make_pair(llvm::CmpInst::FCMP_ONE, OPR_NE);
                default:                    return std::nullopt;
            }
        };

        const auto maybeComparison = resolveComparisonOperator();
        if (!maybeComparison) return nullptr;
    
        const auto& [predicate, label] = *maybeComparison;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::CmpInst::Create(
            llvm::Instruction::FCmp, 
            predicate, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }

    llvm::Value* ComparisonExpressionNode::generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {
        LOG_WARN("IR Code Generation ......");
        const auto resolveComparisonOperator = [this]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>> {
            switch (operatorType) {
                case EQUALITY:              return std::make_pair(llvm::CmpInst::ICMP_EQ, OPR_EQ);
                case NOT_EQUALITY:          return std::make_pair(llvm::CmpInst::ICMP_NE, OPR_NE);
                default:                    return std::nullopt;
            }
        };

        const auto maybeComparison = resolveComparisonOperator();
        if (!maybeComparison) return nullptr;
    
        const auto& [predicate, label] = *maybeComparison;
        auto* currentBlock = astContext.getBuilder().GetInsertBlock();

        return llvm::CmpInst::Create(
            llvm::Instruction::ICmp, 
            predicate, 
            lhsValue, 
            rhsValue, 
            llvm::Twine(label), 
            currentBlock
        );
    }


    llvm::Value* ComparisonExpressionNode::generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {
        LOG_WARN("IR Code Generation ......");
        const auto resolveComparisonOperator = [this]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>> {
            switch (operatorType) {
                case GREATER_THAN:         return std::make_pair(llvm::CmpInst::ICMP_SGT, OPR_GT);
                case LESS_THAN:            return std::make_pair(llvm::CmpInst::ICMP_SLT, OPR_LT);
                case GREATER_THAN_EQUAL:   return std::make_pair(llvm::CmpInst::ICMP_SGE, OPR_GE);
                case LESS_THAN_EQUAL:      return std::make_pair(llvm::CmpInst::ICMP_SLE, OPR_LE);
                case EQUALITY:             return std::make_pair(llvm::CmpInst::ICMP_EQ,  OPR_EQ);
                case NOT_EQUALITY:         return std::make_pair(llvm::CmpInst::ICMP_NE,  OPR_NE);
                default:                   return std::nullopt;
            }
        };

        auto& builder = astContext.getBuilder();
        auto extractStructField = [&](llvm::Value* value, const std::string& labelPrefix) -> llvm::Value* {        
            if (llvm::isa<llvm::Constant>(value)) {
                const auto charType = astContext.findType(value);
                auto stackVar = charType->createInstance(labelPrefix + "_stack");
                charType->assignTo(stackVar, value);
                auto loadedStruct = builder.CreateLoad(stackVar->getType()->getPointerElementType(), stackVar, labelPrefix + ".load");
                return builder.CreateExtractValue(loadedStruct, {0}, labelPrefix + ".short");
            }
            return builder.CreateExtractValue(value, {0}, labelPrefix + ".short");
        };


        const auto maybeComparison = resolveComparisonOperator();
        if (!maybeComparison) return nullptr;

        const auto& [predicate, label] = *maybeComparison;
        auto* lhsExtractValue = extractStructField(lhsValue, "lhs");
        auto* rhsExtractValue = extractStructField(rhsValue, "rhs");

        return llvm::CmpInst::Create(
            llvm::Instruction::ICmp, 
            predicate, 
            lhsExtractValue, 
            rhsExtractValue, 
            llvm::Twine(label), 
            builder.GetInsertBlock()
        );
    }

    llvm::Value* ComparisonExpressionNode::generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {
        LOG_WARN("IR Code Generation ......");
        const auto resolveComparisonOperator = [this]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>> {
            switch (operatorType) {
                case GREATER_THAN:         return std::make_pair(llvm::CmpInst::ICMP_SGT, OPR_GT);
                case LESS_THAN:            return std::make_pair(llvm::CmpInst::ICMP_SLT, OPR_LT);
                case GREATER_THAN_EQUAL:   return std::make_pair(llvm::CmpInst::ICMP_SGE, OPR_GE);
                case LESS_THAN_EQUAL:      return std::make_pair(llvm::CmpInst::ICMP_SLE, OPR_LE);
                case EQUALITY:             return std::make_pair(llvm::CmpInst::ICMP_EQ,  OPR_EQ);
                case NOT_EQUALITY:         return std::make_pair(llvm::CmpInst::ICMP_NE,  OPR_NE);
                default:                   return std::nullopt;
            }
        };

        auto* module = astContext.getModule();
        auto& builder = astContext.getBuilder();

        llvm::Function* strcmpFunc = module->getFunction("strcmp");
        if (!strcmpFunc) {
            llvm::FunctionType* strcmpType = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, false);
            strcmpFunc = llvm::Function::Create(strcmpType, llvm::Function::ExternalLinkage, "strcmp", module);
        }

        const auto maybeComparison = resolveComparisonOperator();
        if (!maybeComparison) return nullptr;

        const auto& [predicate, label] = *maybeComparison;
        auto* result = builder.CreateCall(strcmpFunc, {lhsValue, rhsValue}, "strcmp_result");

        return llvm::CmpInst::Create(
            llvm::Instruction::ICmp, 
            predicate, 
            result, 
            llvm::ConstantInt::get(result->getType(), 0), 
            llvm::Twine(label), 
            builder.GetInsertBlock()
        );
    }

    llvm::Value* ComparisonExpressionNode::generateEnumCode(llvm::Value* lhsValue, llvm::Value* rhsValue, const AstContext& astContext) const {
        LOG_WARN("IR Code Generation ......");
        const auto resolveEnumPredicate = [this]() noexcept -> std::optional<std::pair<llvm::CmpInst::Predicate, const char*>>  {
            switch (operatorType) {
                case EQUALITY:      return std::make_pair(llvm::CmpInst::ICMP_EQ, OPR_EQ);
                case NOT_EQUALITY:  return std::make_pair(llvm::CmpInst::ICMP_NE, OPR_NE);
                default:            return std::nullopt;
            }
        };

        auto& builder = astContext.getBuilder();
        auto extractField = [&](llvm::Value* value, const std::string& labelPrefix) -> llvm::Value* {        
            if (llvm::isa<llvm::Constant>(value)) {
                const auto enumType = astContext.findType(value);
                auto stackVar = enumType->createInstance(labelPrefix + "_stack");
                enumType->assignTo(stackVar, value);
                auto* loadedStruct = builder.CreateLoad(stackVar->getType()->getPointerElementType(), stackVar, labelPrefix + ".load");
                return builder.CreateExtractValue(loadedStruct, {0}, labelPrefix + ".short");
            }
            return builder.CreateExtractValue(value, {0}, labelPrefix + ".short");
        };
        
        const auto maybeComparison = resolveEnumPredicate();
        if (!maybeComparison) return nullptr;

        const auto& [predicate, label] = *maybeComparison;
        auto* lhsExtractValue = extractField(lhsValue, "lhs");
        auto* rhsExtractValue = extractField(rhsValue, "rhs");

        return llvm::CmpInst::Create(
            llvm::Instruction::ICmp, 
            predicate, 
            lhsExtractValue, 
            rhsExtractValue, 
            llvm::Twine(label), 
            builder.GetInsertBlock()
        );
    }

    std::unique_ptr<Node> ComparisonExpressionNode::clone() const {
        return std::make_unique<ComparisonExpressionNode>(
            operatorType, 
            this->leftOperand->clone(), 
            this->rightOperand->clone()
        ); 
    }

}