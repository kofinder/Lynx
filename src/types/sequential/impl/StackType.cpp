
#include "sequential/StackType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* StackType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Type* StackType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* StackType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* StackType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }
    
    llvm::Value* StackType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked...");
        std::cout << "ELEMENT TYPE ===>" << dataTypeToString(elementType->getTypeTag()) << std::endl;
        return nullptr;    
    }  
    
    llvm::Value* StackType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* StackType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* StackType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool StackType::equals(const BaseType* other) const {
        return dynamic_cast<const StackType*>(other) != nullptr;
    }

    std::string StackType::getDebugName() const {
        return "float";
    }

    llvm::DIType* StackType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t StackType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t StackType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags StackType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}