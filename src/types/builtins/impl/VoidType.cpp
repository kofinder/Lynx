#include "builtins/VoidType.hpp"
#include <context/AstContext.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include "visitor/TypeVisitor.hpp"

namespace LynxTypes {

    llvm::Type* VoidType::computeLLVMType() const {
        return llvm::Type::getVoidTy(astContext->getLLVMContext());
    }

    llvm::Type* VoidType::getLLVMPointerType() const {
        return llvm::PointerType::get(astContext->getLLVMContext(), 0);
    }

    llvm::Value* VoidType::getDefaultValue() {
        return nullptr;
    }

    llvm::Value* VoidType::createInstance(std::string variableName) {
        return nullptr;
    }

    llvm::Value* VoidType::createValue(LValueType value) const {
        astContext->reportError(makeRuntimeError("VectorType createValue(Lvalue) doesn't support this createValue signature."));
        return nullptr;
    }        

    llvm::Value* VoidType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return nullptr;
    }

    const BaseType* VoidType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* VoidType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool VoidType::equals(const BaseType* other) const {
        return dynamic_cast<const VoidType*>(other) != nullptr;
    }
    
    std::string VoidType::getDebugName() const {
        return "void";
    }

    llvm::DIType* VoidType::getDIType(llvm::DIScope* scope) const {
        auto& builder = astContext->getDebugBuilder();
        return builder.createUnspecifiedType("void");
    }

    uint64_t VoidType::getDebugSizeInBits() const {
        return 0;
    }

    uint32_t VoidType::getDebugAlignInBits() const {
        return 0;
    }

    llvm::DINode::DIFlags VoidType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }
}

