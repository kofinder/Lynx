
#include "sequential/SetType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* SetType::computeLLVMType() const { return nullptr; }

    llvm::Type* SetType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* SetType::getDefaultValue() { return nullptr; }

    llvm::Value* SetType::createInstance(const std::string& /*variableName*/) { return nullptr; }

    llvm::Value* SetType::createValue(std::vector<llvm::Value*> /*values*/) const { return nullptr; }  
    
    llvm::Value* SetType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool SetType::equals(const BaseType* other) const {
        return dynamic_cast<const SetType*>(other) != nullptr;
    }

    const BaseType* SetType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* SetType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* SetType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t SetType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t SetType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags SetType::getDIFlags() const { return llvm::DINode::FlagZero; }

}