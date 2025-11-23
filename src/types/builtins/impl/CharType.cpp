#include "builtins/CharType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/CharacterMethodResolver.hpp"

namespace LynxTypes {

    llvm::StructType* CharType::cachedType = nullptr;

    llvm::Type* CharType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureCharType);
            cachedType->setBody(llvm::Type::getInt8Ty(getLLVMContext()));
        }
    
        return cachedType;
    }

    llvm::Type* CharType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* CharType::getDefaultValue() {
        return llvm::ConstantAggregateZero::get(computeLLVMType());
    }

    llvm::Value* CharType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        llvm::Value* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureCharType));
            allocaInst->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* CharType::createValue(LValueType value) const {
        auto& builder = getContext()->getBuilder();
        if (std::holds_alternative<char>(value)) {
            const char charValue = std::get<char>(value);
            auto* stuctTy = llvm::cast<llvm::StructType>(computeLLVMType());
            return llvm::ConstantStruct::get(stuctTy, {builder.getInt8(charValue)});
        }

        std::cerr << "Unsupported value type!\n";
        return nullptr;
    }

    llvm::Value* CharType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return getBuilder().CreateStore(rhs, lhs);
    }

    void CharType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* CharType::getOrCreateResolver() const { return nullptr; }

    llvm::Value* CharType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        auto* resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
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
