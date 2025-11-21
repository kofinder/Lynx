
#include "wrapper/PointerType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* PointerType::computeLLVMType() const { return nullptr; }

    llvm::Type* PointerType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* PointerType::getDefaultValue() { return nullptr; }

    llvm::Value* PointerType::createInstance(std::string /*variableName*/) { return nullptr; }
 
    llvm::Value* PointerType::assignTo(llvm::Value* /*lhs*/, llvm::Value* /*rhs*/) { return nullptr; }

    llvm::Value* PointerType::createValue(LValueType /*value*/) const { return nullptr; }

    bool PointerType::equals(const BaseType* /*other*/) const { return false; }

    const BaseType* PointerType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* PointerType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* PointerType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t PointerType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t PointerType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags PointerType::getDIFlags() const { return llvm::DINode::FlagZero; }

}