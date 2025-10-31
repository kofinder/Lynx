#include <logger/Logger.hpp>
#include <types/tmpl/TypeChecker.hpp>
#include "ComparisonExpressionNode.hpp"


using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxTypes;


namespace LynxAst {


    llvm::Value* ComparisonExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("ComparisonExpressionNode code generation ......");

        if (!leftOperand || !rightOperand) {
            LOG_ERROR("Null operand detected in ComparisonExpressionNode");
            return nullptr;
        }

        llvm::Value* lhsValue = leftOperand->generateCode(astContext->createContext());
        llvm::Value* rhsValue = rightOperand->generateCode(astContext->createContext());

        if (!lhsValue || !rhsValue) {
            LOG_ERROR("Failed to generate code for operands.");
            return nullptr;
        }

        auto lhsBaseType = astContext->findType(lhsValue);
        auto rhsBaseType = astContext->findType(rhsValue);

        auto* lhsType = lhsBaseType->getLLVMType();
        auto* rhsType = rhsBaseType->getLLVMType();

        llvm::errs() << "Compare LSH vs RHS Type:"; lhsType->print(llvm::outs()); 
        llvm::errs() << " <> "; rhsType->print(llvm::outs()); llvm::errs() << "\n";

        if (lhsType != rhsType) {
            LOG_ERROR("Operands have different types. Left operand type:  Right operand type:");
            return nullptr;
        }
        
        // Use helper function to check types
        if (TypeChecker::is<ShortType>(lhsType) || 
            TypeChecker::is<IntegerType>(lhsType) || 
            TypeChecker::is<LongType>(lhsType) || 
            TypeChecker::is<ByteType>(lhsType)) {
            return generateIntegerCode(lhsValue, rhsValue, astContext.get());
        }
        if (TypeChecker::is<FloatType>(lhsType) || TypeChecker::is<DoubleType>(lhsType)) {
            return generateDoubleCode(lhsValue, rhsValue, astContext.get());
        }
        if (TypeChecker::is<BooleanType>(lhsType)) {
            return generateBooleanCode(lhsValue, rhsValue, astContext.get());
        }
        if (TypeChecker::is<StringType>(lhsType)) {
            return generateStringCode(lhsValue, rhsValue, astContext.get());
        }
        if (TypeChecker::is<CharType>(lhsType)) {
            return generateCharCode(lhsValue, rhsValue, astContext.get());
        }
        if (TypeChecker::is<EnumType>(lhsType)) {
            return generateEnumCode(lhsValue, rhsValue, astContext.get());
        }
       
        LOG_ERROR("Unsupported type in ComparisonExpressionNode.");
        return nullptr;
    }

    llvm::Value* ComparisonExpressionNode::generateIntegerCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_WARN("Procced ......");

        auto label = "compare";
        llvm::CmpInst::Predicate predicate;
        bool isUnsigned = lhsValue->getType()->isIntegerTy() && !lhsValue->getType()->getScalarSizeInBits(); // Example check

        switch (this->operatorType) {
            case GREATER_THAN:
                predicate = isUnsigned ? llvm::CmpInst::ICMP_UGT : llvm::CmpInst::ICMP_SGT;
                label = MetadataTypeConstants::oprGreaterThanTmp;
                break;
            case LESS_THAN:
                predicate = isUnsigned ? llvm::CmpInst::ICMP_ULT : llvm::CmpInst::ICMP_SLT;
                label = MetadataTypeConstants::oprLessThanTmp;
                break;
            case GREATER_THAN_EQUAL:
                predicate = isUnsigned ? llvm::CmpInst::ICMP_UGE : llvm::CmpInst::ICMP_SGE;
                label = MetadataTypeConstants::oprGreaterEqualTmp;
                break;
            case LESS_THAN_EQUAL:
                predicate = isUnsigned ? llvm::CmpInst::ICMP_ULE : llvm::CmpInst::ICMP_SLE;
                label = MetadataTypeConstants::oprLessEqualTmp;
                break;
            case EQUALITY:
                predicate = llvm::CmpInst::ICMP_EQ; // Equality is same for both signed/unsigned
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::ICMP_NE; // Not equal is same for both signed/unsigned
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr;
        }

        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        return llvm::CmpInst::Create(llvm::Instruction::ICmp, predicate, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* ComparisonExpressionNode::generateDoubleCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_WARN("Procced ......");

        auto label = "compare";
        llvm::CmpInst::Predicate predicate;

        switch (this->operatorType) {
            case GREATER_THAN:
                predicate = llvm::CmpInst::FCMP_OGT;
                label = MetadataTypeConstants::oprGreaterThanTmp;
                break;
            case LESS_THAN:
                predicate = llvm::CmpInst::FCMP_OLT;
                label = MetadataTypeConstants::oprLessThanTmp;
                break;
            case GREATER_THAN_EQUAL:
                predicate = llvm::CmpInst::FCMP_OGE;
                label = MetadataTypeConstants::oprGreaterEqualTmp;
                break;
            case LESS_THAN_EQUAL:
                predicate = llvm::CmpInst::FCMP_OLE;
                label = MetadataTypeConstants::oprLessEqualTmp;
                break;
            case EQUALITY:
                predicate = llvm::CmpInst::FCMP_OEQ;
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::FCMP_ONE;
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr;
        }

        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        return llvm::CmpInst::Create(llvm::Instruction::FCmp, predicate, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* ComparisonExpressionNode::generateBooleanCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_INFO("Procced ......");

        auto label = "compareopr";
        llvm::CmpInst::Predicate predicate;

        switch (this->operatorType) {
            case EQUALITY:
                predicate = llvm::CmpInst::ICMP_EQ;
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::ICMP_NE;
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr; // Invalid operation for boolean
        }

        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        return llvm::CmpInst::Create(llvm::Instruction::ICmp, predicate, lhsValue, rhsValue, llvm::Twine(label), currentBlock);
    }


    llvm::Value* ComparisonExpressionNode::generateCharCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_INFO("Procced ......");

        auto label = "compareopr";
        llvm::CmpInst::Predicate predicate;

        switch (this->operatorType) {
            case GREATER_THAN:
                predicate = llvm::CmpInst::ICMP_SGT;  // Signed greater than
                label = MetadataTypeConstants::oprGreaterThanTmp;
                break;
            case LESS_THAN:
                predicate = llvm::CmpInst::ICMP_SLT;
                label = MetadataTypeConstants::oprLessThanTmp;
                break;
            case GREATER_THAN_EQUAL:
                predicate = llvm::CmpInst::ICMP_SGE;
                label = MetadataTypeConstants::oprGreaterEqualTmp;
                break;
            case LESS_THAN_EQUAL:
                predicate = llvm::CmpInst::ICMP_SLE;
                label = MetadataTypeConstants::oprLessEqualTmp;
                break;
            case EQUALITY:
                predicate = llvm::CmpInst::ICMP_EQ;
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::ICMP_NE;
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr;
        }

        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        auto extractStructField = [&](llvm::Value* value, const std::string& labelPrefix) -> llvm::Value* {        
            if (llvm::isa<llvm::Constant>(value)) {
                const auto charType = astContext->findType(value);
                auto stackVar = charType->createInstance(labelPrefix + "_stack");
                charType->assignTo(stackVar, value);
                auto loadedStruct = builder.CreateLoad(stackVar->getType()->getPointerElementType(), stackVar, labelPrefix + ".load");
                return builder.CreateExtractValue(loadedStruct, {0}, labelPrefix + ".short");
            }
        
            return builder.CreateExtractValue(value, {0}, labelPrefix + ".short");
        };
        
        llvm::Value* lhsExtractValue = extractStructField(lhsValue, "lhs");
        llvm::Value* rhsExtractValue = extractStructField(rhsValue, "rhs");

        return llvm::CmpInst::Create(llvm::Instruction::ICmp, predicate, lhsExtractValue, rhsExtractValue, llvm::Twine(label), currentBlock);
    }

    llvm::Value* ComparisonExpressionNode::generateStringCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_INFO("Procced ......");

        auto label = "compareopr";
        llvm::CmpInst::Predicate predicate;

        switch (this->operatorType) {
            case GREATER_THAN:
                predicate = llvm::CmpInst::ICMP_SGT;  // strcmp > 0 means lhs > rhs
                label = MetadataTypeConstants::oprGreaterThanTmp;
                break;
            case LESS_THAN:
                predicate = llvm::CmpInst::ICMP_SLT;  // strcmp < 0 means lhs < rhs
                label = MetadataTypeConstants::oprLessThanTmp;
                break;
            case GREATER_THAN_EQUAL:
                predicate = llvm::CmpInst::ICMP_SGE;
                label = MetadataTypeConstants::oprGreaterEqualTmp;
                break;
            case LESS_THAN_EQUAL:
                predicate = llvm::CmpInst::ICMP_SLE;
                label = MetadataTypeConstants::oprLessEqualTmp;
                break;
            case EQUALITY:
                predicate = llvm::CmpInst::ICMP_EQ;  // strcmp == 0 means equal
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::ICMP_NE;
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr;
        }

        auto* module = astContext->getModule();
        auto& builder = astContext->getBuilder();

        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        // Declare `strcmp` function
        llvm::Function* strcmpFunc = module->getFunction("strcmp");
        if (!strcmpFunc) {
            llvm::FunctionType* strcmpType = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, false);
            strcmpFunc = llvm::Function::Create(strcmpType, llvm::Function::ExternalLinkage, "strcmp", module);
        }

        // Call `strcmp(lhs, rhs)`
        llvm::Value* result = builder.CreateCall(strcmpFunc, {lhsValue, rhsValue}, "strcmp_result");

        return llvm::CmpInst::Create(llvm::Instruction::ICmp, predicate, result, llvm::ConstantInt::get(result->getType(), 0), llvm::Twine(label), currentBlock);
    }


    llvm::Value* ComparisonExpressionNode::generateEnumCode(llvm::Value* lhsValue, llvm::Value* rhsValue, AstContext* astContext) {
        LOG_INFO("Procced ......");

        auto label = "compareopr";
        llvm::CmpInst::Predicate predicate;

        switch (this->operatorType) {
            case EQUALITY:
                predicate = llvm::CmpInst::ICMP_EQ; // Equality is same for both signed/unsigned
                label = MetadataTypeConstants::oprEqualityTmp;
                break;
            case NOT_EQUALITY:
                predicate = llvm::CmpInst::ICMP_NE; // Not equal is same for both signed/unsigned
                label = MetadataTypeConstants::oprNotEqualityTmp;
                break;
            default:
                return nullptr;
        }

        auto& builder = astContext->getBuilder();
        llvm::BasicBlock* currentBlock = builder.GetInsertBlock();

        auto extractField = [&](llvm::Value* value, const std::string& labelPrefix) -> llvm::Value* {        
            if (llvm::isa<llvm::Constant>(value)) {
                const auto enumType = astContext->findType(value);
                auto stackVar = enumType->createInstance(labelPrefix + "_stack");
                enumType->assignTo(stackVar, value);
                auto* loadedStruct = builder.CreateLoad(stackVar->getType()->getPointerElementType(), stackVar, labelPrefix + ".load");
                return builder.CreateExtractValue(loadedStruct, {0}, labelPrefix + ".short");
            }
            return builder.CreateExtractValue(value, {0}, labelPrefix + ".short");
        };
        
        llvm::Value* lhsExtractValue = extractField(lhsValue, "lhs");
        llvm::Value* rhsExtractValue = extractField(rhsValue, "rhs");

        // llvm::errs() << "Lhs value ==>"; lhsValue->print(llvm::outs()); llvm::errs() << "\n";
        // llvm::errs() << "Rhs value ==>"; rhsValue->print(llvm::outs()); llvm::errs() << "\n";
        // llvm::errs() << "Lhs extract value ==>"; lhsExtractValue->print(llvm::outs()); llvm::errs() << "\n";
        // llvm::errs() << "Rhs extract value ==>"; rhsExtractValue->print(llvm::outs()); llvm::errs() << "\n";

        return llvm::CmpInst::Create(llvm::Instruction::ICmp, predicate, lhsExtractValue, rhsExtractValue, llvm::Twine(label), currentBlock);
    }

    std::unique_ptr<Node> ComparisonExpressionNode::clone() const {
        return std::make_unique<ComparisonExpressionNode>(
            this->operatorType, 
            this->leftOperand->clone(), 
            this->rightOperand->clone()
        ); 
    }

}