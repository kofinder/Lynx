
#include "sequential/QueueType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* QueueType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Type* QueueType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* QueueType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* QueueType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* QueueType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked...");
        std::cout << "ELEMENT TYPE ===>" << toString(elementType->getTypeTag()) << std::endl;
        return nullptr;    
    }  
        

    llvm::Value* QueueType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        // Ensure that both lhs and rhs are valid before proceeding with the assignment
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* QueueType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* QueueType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool QueueType::equals(const BaseType* other) const {
        return dynamic_cast<const QueueType*>(other) != nullptr;
    }

    std::string QueueType::getDebugName() const {
        return "float";
    }

    llvm::DIType* QueueType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t QueueType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t QueueType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags QueueType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}