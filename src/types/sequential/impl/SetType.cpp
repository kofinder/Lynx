
#include "sequential/SetType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* SetType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatTy(context);
    }

    llvm::Type* SetType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getFloatPtrTy(context);
    }

    llvm::Value* SetType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* SetType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* floatType = this->getLLVMType();
        auto var = builder.CreateAlloca(floatType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::floatType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        
        return var;
    }

    llvm::Value* SetType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked...");
        std::cout << "ELEMENT TYPE ===>" << toString(elementType->getTypeTag()) << std::endl;
        return nullptr;    
    }  
    
    llvm::Value* SetType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    const BaseType* SetType::createWithStatic(bool newIsStatic) const {
        auto clone = std::make_shared<SetType>(astContext);
        clone->setElementType(elementType);
        clone->createWithStatic(newIsStatic);
        clone->createWithConst(isConst());
        return clone.get();
    }

    const BaseType* SetType::createWithConst(bool newIsConst) const {
        auto clone = std::make_shared<SetType>(astContext);
        clone->setElementType(elementType);
        clone->setConst(newIsConst);
        clone->setStatic(isStatic());
        return clone.get();
    }

    bool SetType::equals(const BaseType* other) const {
        return dynamic_cast<const SetType*>(other) != nullptr;
    }

    std::string SetType::getDebugName() const {
        return "vector<" + elementType->getDebugName() + ">";
    }

    llvm::DIType* SetType::getDIType(llvm::DIScope* scope) const {
        return elementType->getDIType(scope); 
    }

    uint64_t SetType::getDebugSizeInBits() const {
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t SetType::getDebugAlignInBits() const {
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags SetType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}