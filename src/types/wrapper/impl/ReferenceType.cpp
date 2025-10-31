
#include "wrapper/ReferenceType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* ReferenceType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatTy(context);
    }

    llvm::Type* ReferenceType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatPtrTy(context);
    }

    llvm::Value* ReferenceType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0.0f;
        return this->createValue(LValueType);
    }

    llvm::Value* ReferenceType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* floatType = this->getLLVMType();
        auto var = builder.CreateAlloca(floatType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::floatType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        
        return var;
    }

    llvm::Value* ReferenceType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }


    llvm::Value* ReferenceType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* ReferenceType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* ReferenceType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool ReferenceType::equals(const BaseType* other) const {
        return dynamic_cast<const ReferenceType*>(other) != nullptr;
    }

    std::string ReferenceType::getDebugName() const {
        return "refrence";
    }

    llvm::DIType* ReferenceType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t ReferenceType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t ReferenceType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags ReferenceType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}