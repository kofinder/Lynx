#include <llvm/IR/DebugInfoMetadata.h>
#include <context/AstContext.hpp>
#include "userdefined/FunctionType.hpp"

using namespace LynxContext;

namespace LynxTypes {

    llvm::Type* FunctionType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getInt32Ty(context);
    }

    llvm::Type* FunctionType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getInt32PtrTy(context);
    }

    llvm::Value* FunctionType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0;
        return this->createValue(LValueType);
    }

    llvm::Value* FunctionType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* intType = this->getLLVMType();
        auto var = builder.CreateAlloca(intType, nullptr, variableName); 

        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::intType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }       
       
        return var;
    }
 
    llvm::Value* FunctionType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* FunctionType::createWithStatic(bool newIsStatic) const {
        auto clone = std::make_shared<FunctionType>(astContext);
        clone->createWithStatic(newIsStatic);
        clone->createWithConst(isConst());
        return clone.get();
    }

    const BaseType* FunctionType::createWithConst(bool newIsConst) const {
        auto clone = std::make_shared<FunctionType>(astContext);
        clone->setConst(newIsConst);
        clone->setStatic(isStatic());
        return clone.get();
    }
    
    bool FunctionType::equals(const BaseType* other) const {
        auto* otherInteger = dynamic_cast<const FunctionType*>(other);
        if (!otherInteger) return false;
        return this->isConst() == otherInteger->isConst() &&
               this->isStatic() == otherInteger->isStatic();
    }
    

    std::string FunctionType::getDebugName() const {
        LOG_INFO("Invoked...");
        return isConst() ? "const int" : "int";
    }

    llvm::DIType* FunctionType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "int"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_signed
        );
    }

    uint64_t FunctionType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 32; // 32-bit signed integer
    }

    uint32_t FunctionType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32; // Common alignment for 32-bit integers
    }
    
    llvm::DINode::DIFlags FunctionType::getDIFlags() const {
        LOG_INFO("Invoked...");
        llvm::DINode::DIFlags flags = llvm::DINode::FlagZero;
    
        if (isStatic()) {
            flags |= llvm::DINode::FlagStaticMember;
        }
    
        return flags;
    }
    
}