
#include "associative/DictionaryType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* DictionaryType::computeLLVMType() const { return nullptr;  }

    llvm::Type* DictionaryType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* DictionaryType::getDefaultValue() { return nullptr; }

    llvm::Value* DictionaryType::createInstance(const std::string& /*variableName*/) { return nullptr; }

    llvm::Value* DictionaryType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*pairs*/) const { return nullptr; }     

    llvm::Value* DictionaryType::assignTo(llvm::Value* /*unused*/, llvm::Value* /*unused*/)  { return nullptr; }

    bool DictionaryType::equals(const BaseType* other) const {
        return dynamic_cast<const DictionaryType*>(other) != nullptr;
    }

    const BaseType* DictionaryType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* DictionaryType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* DictionaryType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t DictionaryType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t DictionaryType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags DictionaryType::getDIFlags() const { return llvm::DINode::FlagZero; }
}