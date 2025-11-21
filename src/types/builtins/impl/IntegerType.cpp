#include "builtins/IntegerType.hpp"
#include <llvm/IR/DebugInfoMetadata.h>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/IntMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* IntegerType::computeLLVMType() const {
        return llvm::Type::getInt32Ty(astContext->getLLVMContext());
    }

    llvm::Type* IntegerType::getLLVMPointerType() const {
        auto* intTy = llvm::Type::getInt32Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(intTy->getContext(), 0);
    }

    llvm::Value* IntegerType::getDefaultValue() {
        const LValueType value = 0;
        return createValue(value);
    }

    llvm::Value* IntegerType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* intType = this->getLLVMType();
        auto* var = builder.CreateAlloca(intType, nullptr, variableName); 

        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::intType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }       
       
        return var;
    }

    llvm::Value* IntegerType::createValue(LValueType value) const {
        if(std::holds_alternative<int>(value)) {
            auto& context = astContext->getLLVMContext();
            const int intValue = std::get<int>(value); 
            return llvm::ConstantInt::get(context, llvm::APInt(INT_BIT_WIDTH, intValue));
        }

        LOG_ERROR("Unsupported value type");
        return nullptr;
    }
 
    llvm::Value* IntegerType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void IntegerType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* IntegerType::getOrCreateResolver() const { return IntMethodResolver::create(); }

    llvm::Value* IntegerType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver) resolver = IntMethodResolver::create();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    
    bool IntegerType::equals(const BaseType* other) const {
        const auto* otherInteger = dynamic_cast<const IntegerType*>(other);
        if (!otherInteger) return false;
        return this->isConst() == otherInteger->isConst() &&
               this->isStatic() == otherInteger->isStatic();
    }

    const BaseType* IntegerType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* IntegerType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* IntegerType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t IntegerType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t IntegerType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags IntegerType::getDIFlags() const { return llvm::DINode::FlagZero; }
}

