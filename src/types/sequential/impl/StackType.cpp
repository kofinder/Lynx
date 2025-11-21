
#include "sequential/StackType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* StackType::computeLLVMType() const { return nullptr; }

    llvm::Type* StackType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* StackType::getDefaultValue() { return nullptr; }

    llvm::Value* StackType::createInstance(std::string /*variableName*/) { return nullptr; }
    
    llvm::Value* StackType::createValue(std::vector<llvm::Value*> /*values*/) const { return nullptr; }  
    
    llvm::Value* StackType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool StackType::equals(const BaseType* other) const {
        return dynamic_cast<const StackType*>(other) != nullptr;
    }

    const BaseType* StackType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* StackType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* StackType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t StackType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t StackType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags StackType::getDIFlags() const { return llvm::DINode::FlagZero; }

}