#include "builtins/VoidType.hpp"
#include <context/AstContext.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>

using namespace LynxContext;

namespace LynxTypes {

    llvm::Type* VoidType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getVoidTy(context);
    }

    llvm::Type* VoidType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getInt8PtrTy(context);
    }

    llvm::Value* VoidType::getDefaultValue() {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* VoidType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    llvm::Value* VoidType::createValue(LValueType value) const {
        astContext->reportError(makeRuntimeError("VectorType createValue(Lvalue) doesn't support this createValue signature."));
        return nullptr;
    }        

    llvm::Value* VoidType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* VoidType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* VoidType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }
    
    bool VoidType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const VoidType*>(other) != nullptr;
    }
    
    std::string VoidType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "void";
    }

    llvm::DIType* VoidType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        return builder.createUnspecifiedType("void");
    }

    uint64_t VoidType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 0; // Void has no size
    }

    uint32_t VoidType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 0; // Void has no alignment
    }

    llvm::DINode::DIFlags VoidType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}

