
#include "wrapper/ReferenceType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* ReferenceType::computeLLVMType() const { return nullptr; }

    llvm::Type* ReferenceType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* ReferenceType::getDefaultValue() { return nullptr; }

    llvm::Value* ReferenceType::createInstance(const std::string& /*variableName*/) { return nullptr; }
 
    llvm::Value* ReferenceType::assignTo(llvm::Value* /*lhs*/, llvm::Value* /*rhs*/) { return nullptr; }

    llvm::Value* ReferenceType::createValue(LValueType /*value*/) const { return nullptr; }

    bool ReferenceType::equals(const BaseType* /*other*/) const { return false; }

    const BaseType* ReferenceType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ReferenceType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ReferenceType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ReferenceType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ReferenceType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ReferenceType::getDIFlags() const { return llvm::DINode::FlagZero; }
}