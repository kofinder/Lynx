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

    llvm::Value* VoidType::getDefaultValue() { return nullptr; }

    llvm::Value* VoidType::createInstance(const std::string& /*name*/) { return nullptr; }

    llvm::Value* VoidType::createValue(LValueType /*value*/) const { return nullptr; }        

    llvm::Value* VoidType::assignTo(llvm::Value* /*lhs*/, llvm::Value* /*rhs*/) { return nullptr; }

    bool VoidType::equals(const BaseType* other) const {
        return dynamic_cast<const VoidType*>(other) != nullptr;
    }

    const BaseType* VoidType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* VoidType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* VoidType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t VoidType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t VoidType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags VoidType::getDIFlags() const { return llvm::DINode::FlagZero; }
}

