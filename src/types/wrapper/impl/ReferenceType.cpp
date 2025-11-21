
#include "wrapper/ReferenceType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* ReferenceType::computeLLVMType() const {
        return nullptr;
    }

    llvm::Type* ReferenceType::getLLVMPointerType() const {
        return nullptr;
    }

    llvm::Value* ReferenceType::getDefaultValue() {
        return nullptr;
    }

    llvm::Value* ReferenceType::createInstance(std::string variableName) {
        return nullptr;
    }

    llvm::Value* ReferenceType::createValue(LValueType value) const {
        return nullptr;
    }


    llvm::Value* ReferenceType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* ReferenceType::createWithStatic(bool) const {
        return nullptr;
    }

    const BaseType* ReferenceType::createWithConst(bool) const {
        return nullptr;
    }

    bool ReferenceType::equals(const BaseType* other) const {
        return dynamic_cast<const ReferenceType*>(other) != nullptr;
    }

    std::string ReferenceType::getDebugName() const {
        return "refrence";
    }

    llvm::DIType* ReferenceType::getDIType(llvm::DIScope* /*scope*/) const {
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