
#include "wrapper/PointerType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* PointerType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatTy(context);
    }

    llvm::Type* PointerType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatPtrTy(context);
    }

    llvm::Value* PointerType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0.0f;
        return this->createValue(LValueType);
    }

    llvm::Value* PointerType::createInstance(std::string variableName) {
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

    llvm::Value* PointerType::createValue(LValueType value) const {
        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* PointerType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        // Ensure that both lhs and rhs are valid before proceeding with the assignment
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* PointerType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* PointerType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool PointerType::equals(const BaseType* other) const {
        return dynamic_cast<const PointerType*>(other) != nullptr;
    }

    std::string PointerType::getDebugName() const {
        return "pointer";
    }

    llvm::DIType* PointerType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t PointerType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t PointerType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags PointerType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}