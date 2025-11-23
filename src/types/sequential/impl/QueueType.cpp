
#include "sequential/QueueType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* QueueType::computeLLVMType() const { return nullptr; }

    llvm::Type* QueueType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* QueueType::getDefaultValue() { return nullptr; }

    llvm::Value* QueueType::createInstance(const std::string& /*variableName*/) { return nullptr; }

    llvm::Value* QueueType::createValue(std::vector<llvm::Value*> /*values*/) const { return nullptr; }  

    llvm::Value* QueueType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool QueueType::equals(const BaseType* other) const {
        return dynamic_cast<const QueueType*>(other) != nullptr;
    }

    const BaseType* QueueType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* QueueType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* QueueType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t QueueType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t QueueType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags QueueType::getDIFlags() const { return llvm::DINode::FlagZero; }

}