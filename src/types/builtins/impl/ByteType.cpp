#include "builtins/ByteType.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/ByteMethodResolver.hpp"

namespace LynxTypes {
    
    llvm::StructType* ByteType::cachedType = nullptr;

    llvm::Type* ByteType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureByteType);
            cachedType->setBody(llvm::Type::getInt8Ty(context));
        }
    
        return cachedType;
    }

    llvm::Type* ByteType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* ByteType::getDefaultValue() {
        return llvm::ConstantAggregateZero::get(computeLLVMType());
    }

    llvm::Value* ByteType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        llvm::Value* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureByteType));
            allocaInst->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* ByteType::createValue(LValueType value) const {
        if (std::holds_alternative<uint8_t>(value)) {
            auto& builder = getBuilder();
            const uint8_t byteValue = std::get<uint8_t>(value);
            auto* structTy = llvm::cast<llvm::StructType>(computeLLVMType());
            return llvm::ConstantStruct::get(structTy, {builder.getInt8(byteValue)});
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }  

    llvm::Value* ByteType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        return getBuilder().CreateStore(rhs, lhs);
    }

    void ByteType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* ByteType::getOrCreateResolver() const { return nullptr; }

    llvm::Value* ByteType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
    }

    bool ByteType::equals(const BaseType* other) const {
        return dynamic_cast<const ByteType*>(other) != nullptr;
    }

    const BaseType* ByteType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ByteType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ByteType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ByteType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ByteType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ByteType::getDIFlags() const { return llvm::DINode::FlagZero; }
}
