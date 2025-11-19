#include "builtins/ByteType.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/ByteMethodResolver.hpp"

namespace LynxTypes {
    
    llvm::StructType* ByteType::cachedType = nullptr;

    llvm::Type* ByteType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureByteType);
            cachedType->setBody(llvm::Type::getInt8Ty(context));
        }
    
        return cachedType;
    }

    llvm::Type* ByteType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* ByteType::getDefaultValue() {
        LOG_INFO("Invoked...");
        llvm::Type* byteType = computeLLVMType();
        return llvm::ConstantAggregateZero::get(byteType);
    }

    llvm::Value* ByteType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* byteType = computeLLVMType();
        llvm::Value* var = builder.CreateAlloca(byteType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::structureByteType));
            allocaInst->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* ByteType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if (std::holds_alternative<uint8_t>(value)) {
            auto& builder = astContext->getBuilder();
            uint8_t byteValue = std::get<uint8_t>(value);
            llvm::Type* byteType = computeLLVMType();
            llvm::Constant* val = llvm::ConstantStruct::get(static_cast<llvm::StructType*>(byteType), {builder.getInt8(byteValue)});
            return val;
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }  

    llvm::Value* ByteType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void ByteType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* ByteType::getOrCreateResolver() const { 
        if (!resolver) resolver = new ByteMethodResolver();
        return resolver;
    }

    llvm::Value* ByteType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    const BaseType* ByteType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* ByteType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool ByteType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const ByteType*>(other) != nullptr;
    }

    std::string ByteType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "byte";
    }

    llvm::DIType* ByteType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        auto& context = astContext->getLLVMContext();

        // Get the element type debug info (uint8)
        llvm::DIType* elementType = builder.createBasicType(
            "uint8_t",
            8,                    // size in bits
            llvm::dwarf::DW_ATE_unsigned_char
        );

        // Create a struct type with one member (the byte)
        llvm::DIType* structType = builder.createStructType(
            scope,
            getDebugName(),       // struct name
            nullptr,              // file, can be nullptr or real file DIFile*
            0,                    // line number
            8,                    // size in bits (one byte)
            8,                    // alignment in bits
            llvm::DINode::FlagZero,
            nullptr,              // derived from (base types)
            builder.getOrCreateArray({elementType}) // members array
        );

        return structType;
    }

    uint64_t ByteType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 8;  // 1 byte = 8 bits
    }

    uint32_t ByteType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 8;  // byte alignment is 8 bits
    }

    llvm::DINode::DIFlags ByteType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}
