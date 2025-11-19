
#include "sequential/ListType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* ListType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Type* ListType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* ListType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* ListType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* ListType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked...");
        std::cout << "ELEMENT TYPE ===>" << dataTypeToString(elementType->getTypeTag()) << std::endl;
        return nullptr;    
    }  
    

    llvm::Value* ListType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        // Ensure that both lhs and rhs are valid before proceeding with the assignment
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* ListType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* ListType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool ListType::equals(const BaseType* other) const {
        return dynamic_cast<const ListType*>(other) != nullptr;
    }

    std::string ListType::getDebugName() const {
        return "float";
    }

    llvm::DIType* ListType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t ListType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t ListType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags ListType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}