#include "builtins/ShortType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/ShortMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* ShortType::computeLLVMType() const {
        return llvm::Type::getInt16Ty(astContext->getLLVMContext());
    }

    llvm::Type* ShortType::getLLVMPointerType() const {
        auto* shortTy = llvm::Type::getInt16Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(shortTy->getContext(), 0);
    }

    llvm::Value* ShortType::getDefaultValue() {
        const LValueType value = 0;
        return createValue(value);
    }

    llvm::Value* ShortType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* shortType = this->getLLVMType();
        auto* var = builder.CreateAlloca(shortType, nullptr, variableName);

        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::shortType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* ShortType::createValue(LValueType value) const {
        if(std::holds_alternative<short>(value)) {
            auto& context = astContext->getLLVMContext();
            const short shortValue = std::get<short>(value);
            return llvm::ConstantInt::get(context, llvm::APInt(SHORT_BIT_WIDTH, shortValue));
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }

    llvm::Value* ShortType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void ShortType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* ShortType::getOrCreateResolver() const { return ShortMethodResolver::create(); }

    llvm::Value* ShortType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    bool ShortType::equals(const BaseType* other) const {
        return dynamic_cast<const ShortType*>(other) != nullptr;
    }

    const BaseType* ShortType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ShortType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ShortType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ShortType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ShortType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ShortType::getDIFlags() const { return llvm::DINode::FlagZero; }
}