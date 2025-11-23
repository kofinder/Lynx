
#include "associative/MapType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* MapType::computeLLVMType() const { return nullptr; }

    llvm::Type* MapType::getLLVMPointerType() const { return nullptr; }

    llvm::Value* MapType::getDefaultValue() { return nullptr;  }

    llvm::Value* MapType::createInstance(const std::string& /*variableName*/) { return nullptr; }

    llvm::Value* MapType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /* pairs */) const { return nullptr; }     

    llvm::Value* MapType::assignTo(llvm::Value* /*unused*/, llvm::Value* /*unused*/)  { return nullptr; }

    bool MapType::equals(const BaseType* other) const {
        return dynamic_cast<const MapType*>(other) != nullptr;
    }

    const BaseType* MapType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* MapType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* MapType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t MapType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t MapType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags MapType::getDIFlags() const { return llvm::DINode::FlagZero; }
}