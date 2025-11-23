#include "builtins/FloatType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/FloatMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* FloatType::computeLLVMType() const {
        return llvm::Type::getFloatTy(astContext->getLLVMContext());
    }

    llvm::Type* FloatType::getLLVMPointerType() const {
        auto* floatTy = llvm::Type::getFloatTy(astContext->getLLVMContext());
        auto* floatPtr = llvm::PointerType::get(floatTy->getContext(), 0); 
        return floatPtr;
    }

    llvm::Value* FloatType::getDefaultValue() {
        const LValueType value = 0.0F;
        return createValue(value);
    }

    llvm::Value* FloatType::createInstance(const std::string& variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* floatType = getLLVMType();
        auto* var = builder.CreateAlloca(floatType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::floatType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        
        return var;
    }

    llvm::Value* FloatType::createValue(LValueType value) const {
        if(std::holds_alternative<float>(value)) {
            auto& context = astContext->getLLVMContext();
            const float floatValue = std::get<float>(value);
            return llvm::ConstantFP::get(context, llvm::APFloat(floatValue));
        } 
        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* FloatType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
    
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void FloatType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* FloatType::getOrCreateResolver() const { return FloatMethodResolver::create(); }

    llvm::Value* FloatType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    bool FloatType::equals(const BaseType* other) const {
        return dynamic_cast<const FloatType*>(other) != nullptr;
    }

    const BaseType* FloatType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* FloatType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* FloatType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t FloatType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t FloatType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags FloatType::getDIFlags() const { return llvm::DINode::FlagZero; }
}