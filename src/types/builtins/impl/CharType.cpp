#include "builtins/CharType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/CharacterMethodResolver.hpp"

namespace LynxTypes {

    llvm::StructType* CharType::cachedType = nullptr;

    llvm::Type* CharType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureCharType);
            cachedType->setBody(llvm::Type::getInt8Ty(context));
        }
    
        return cachedType;
    }

    llvm::Type* CharType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* CharType::getDefaultValue() {
        auto* charType = computeLLVMType();
        return llvm::ConstantAggregateZero::get(charType);
    }

    llvm::Value* CharType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* charType = computeLLVMType();
        llvm::Value* var = builder.CreateAlloca(charType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureCharType));
            allocaInst->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* CharType::createValue(LValueType value) const {
        auto& builder = astContext->getBuilder();
        if (std::holds_alternative<char>(value)) {
            const char charValue = std::get<char>(value);
            auto* charType = computeLLVMType();
            auto* stuctTy = llvm::cast<llvm::StructType>(charType);
            return llvm::ConstantStruct::get(stuctTy, {builder.getInt8(charValue)});
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }

    llvm::Value* CharType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void CharType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* CharType::getOrCreateResolver() const { 
        return nullptr;
    }

    llvm::Value* CharType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver)  resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    bool CharType::equals(const BaseType* other) const {
        return dynamic_cast<const CharType*>(other) != nullptr;
    }

    const BaseType* CharType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* CharType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* CharType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t CharType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t CharType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags CharType::getDIFlags() const { return llvm::DINode::FlagZero; }
}
