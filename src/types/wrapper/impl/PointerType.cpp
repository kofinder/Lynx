
#include "wrapper/PointerType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* PointerType::computeLLVMType() const {
        return nullptr;
    }

    llvm::Type* PointerType::getLLVMPointerType() const {
        return nullptr;
    }

    llvm::Value* PointerType::getDefaultValue() {
        return nullptr;
    }

    llvm::Value* PointerType::createInstance(std::string variableName) {
        return nullptr;
    }

    llvm::Value* PointerType::createValue(LValueType value) const {
        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* PointerType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        // Ensure that both lhs and rhs are valid before proceeding with the assignment
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* PointerType::createWithStatic(bool) const {
        return nullptr;
    }

    const BaseType* PointerType::createWithConst(bool) const {
        return nullptr;
    }

    bool PointerType::equals(const BaseType* other) const {
        return dynamic_cast<const PointerType*>(other) != nullptr;
    }

    std::string PointerType::getDebugName() const {
        return "pointer";
    }

    llvm::DIType* PointerType::getDIType(llvm::DIScope* /*scope*/) const {
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