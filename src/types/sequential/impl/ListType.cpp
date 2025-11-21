
#include "sequential/ListType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* ListType::computeLLVMType() const { return nullptr; }

    llvm::Type* ListType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* ListType::getDefaultValue() { return nullptr; }

    llvm::Value* ListType::createInstance(std::string /*variableName*/) { return nullptr; }

    llvm::Value* ListType::createValue(std::vector<llvm::Value*> /*values*/) const { return nullptr; }      

    llvm::Value* ListType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool ListType::equals(const BaseType* other) const {
        return dynamic_cast<const ListType*>(other) != nullptr;
    }

    const BaseType* ListType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ListType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ListType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ListType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ListType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ListType::getDIFlags() const { return llvm::DINode::FlagZero; }

}