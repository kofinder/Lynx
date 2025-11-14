
#include "associative/DictionaryType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* DictionaryType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Type* DictionaryType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* DictionaryType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* DictionaryType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* DictionaryType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const {
        LOG_INFO("Invoked...");
        std::cout << "Dictionary Key Type ===>" << dataTypeToString(elementType->getTypeTag()) << std::endl;
        std::cout << "Dictionary ValueType  ===>" << dataTypeToString(elementValue->getTypeTag()) << std::endl;
        return nullptr;       
    }     

    llvm::Value* DictionaryType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* DictionaryType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* DictionaryType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool DictionaryType::equals(const BaseType* other) const {
        return dynamic_cast<const DictionaryType*>(other) != nullptr;
    }

    std::string DictionaryType::getDebugName() const {
        return "DictionaryType";
    }

    llvm::DIType* DictionaryType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t DictionaryType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t DictionaryType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags DictionaryType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}