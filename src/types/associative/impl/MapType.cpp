
#include "associative/MapType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* MapType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Type* MapType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* MapType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* MapType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* MapType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const {
        LOG_INFO("Invoked...");
        std::cout << "Map Key Type ===>" << toString(elementType->getTypeTag()) << std::endl;
        std::cout << "Map ValueType  ===>" << toString(elementValue->getTypeTag()) << std::endl;
        return nullptr;    
    }     

    llvm::Value* MapType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* MapType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* MapType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool MapType::equals(const BaseType* other) const {
        return dynamic_cast<const MapType*>(other) != nullptr;
    }

    std::string MapType::getDebugName() const {
        return "MapType";
    }

    llvm::DIType* MapType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t MapType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t MapType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags MapType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}