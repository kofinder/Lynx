#include "tmpl/CloneNodeTemplate.hpp"
#include "ClazzConstructorExpressionNode.hpp"

namespace LynxAst {
    
    llvm::Value* ClazzConstructorExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("It's called unexpectedly. This method should be handled by the parent node.");
        throw std::runtime_error("It must be handled by the parent node.");    
    }

    llvm::Value* ClazzConstructorExpressionNode::generate(AstContext& astContext, const ClassType& clazzType, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames) {
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();
        auto& builder = astContext.getBuilder();
        auto symbol = astContext.getGlobalContext();

        auto llvmStructType = llvm::cast<llvm::StructType>(clazzType.getLLVMType());
        auto* thisPtr = symbol->findLocalVariable("this");

        llvm::Value* valueToAssign = nullptr;
        if(objectCreationNode.has_value() && objectCreationNode.value()) {
            valueToAssign = objectCreationNode.value()->generateCode(astContext.createContext());
        } else {
            auto it = std::find(argOfNames.begin(), argOfNames.end(), argumentName);
            size_t argIndex = std::distance(argOfNames.begin(), it);
            valueToAssign = argOfValues[argIndex];
        }

        int fieldIndex = clazzType.getFieldIndex(fieldName);
        if (thisPtr->getType()->isPointerTy() && thisPtr->getType()->getPointerElementType()->isIntegerTy(8)) {
            thisPtr = builder.CreateBitCast(thisPtr, llvmStructType->getPointerTo());
        }

        auto* fieldPtr = builder.CreateStructGEP(llvmStructType, thisPtr, fieldIndex, fieldName + "_ptr");
        
        if (valueToAssign->getType() != fieldPtr->getType()->getPointerElementType()) {
            llvm::errs() << "Type mismatch when storing field '" << fieldName << "'\n";
        }

        builder.CreateStore(valueToAssign, fieldPtr);
        
        symbol->registerLocalVariable(fieldName, fieldPtr);

        return fieldPtr;
    }

    std::unique_ptr<Node> ClazzConstructorExpressionNode::clone() const {
        auto clonedNode = Cloneable::cloneOptionalNode(objectCreationNode);
        if (clonedNode.has_value()) {
            return std::make_unique<ClazzConstructorExpressionNode>(fieldName, std::move(clonedNode.value()));
        } else {
            return std::make_unique<ClazzConstructorExpressionNode>(fieldName, argumentName);
        }
    }
    
}