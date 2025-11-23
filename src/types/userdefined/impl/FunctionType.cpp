#include <llvm/IR/DebugInfoMetadata.h>
#include <context/AstContext.hpp>
#include "userdefined/FunctionType.hpp"

using namespace LynxContext;

namespace LynxTypes {

    llvm::Type* FunctionType::computeLLVMType() const { return nullptr; }

    llvm::Type* FunctionType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* FunctionType::getDefaultValue() { return nullptr; }

    llvm::Value* FunctionType::createInstance(const std::string& /*variableName*/) { return nullptr; }
 
    llvm::Value* FunctionType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool FunctionType::equals(const BaseType* other) const {
        const auto* otherInteger = dynamic_cast<const FunctionType*>(other);
        if (!otherInteger) return false;
        return this->isConst() == otherInteger->isConst() && this->isStatic() == otherInteger->isStatic();
    }

    const BaseType* FunctionType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* FunctionType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* FunctionType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t FunctionType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t FunctionType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags FunctionType::getDIFlags() const { return llvm::DINode::FlagZero; }
    
}