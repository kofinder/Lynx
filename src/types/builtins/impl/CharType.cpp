#include "builtins/CharType.hpp"
#include <context/AstContext.hpp>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/CharacterMethodResolver.hpp"

using namespace LynxContext;

namespace LynxTypes {

    llvm::StructType* CharType::cachedType = nullptr;

    llvm::Type* CharType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureCharType);
            cachedType->setBody(llvm::Type::getInt8Ty(context));
        }
    
        return cachedType;
    }

    llvm::Type* CharType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* CharType::getDefaultValue() {
        LOG_INFO("Invoked...");
        auto* charType = computeLLVMType();
        return llvm::ConstantAggregateZero::get(charType);
    }

    llvm::Value* CharType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
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
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        if (std::holds_alternative<char>(value)) {
            char charValue = std::get<char>(value);
            llvm::Type* charType = computeLLVMType();
            llvm::Constant* charConst = llvm::ConstantStruct::get(static_cast<llvm::StructType*>(charType), {builder.getInt8(charValue)});
            return charConst;
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }

    llvm::Value* CharType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    TypeMethodResolver* CharType::getOrCreateResolver() const { 
        if (!resolver) resolver = new CharacterMethodResolver();
        return resolver;
    }

    llvm::Value* CharType::emitMethodCall(llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver)  resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, methodName, args);
    }

    const BaseType* CharType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* CharType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool CharType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const CharType*>(other) != nullptr;
    }

    std::string CharType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "char";
    }

    llvm::DIType* CharType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        // Create a basic type for a signed 8-bit char
        llvm::DIType* elementType = builder.createBasicType(
            "char",
            8,  // size in bits
            llvm::dwarf::DW_ATE_signed_char
        );

        // Create a struct type wrapping the char
        llvm::DIType* structType = builder.createStructType(
            scope,
            getDebugName(),       // struct name
            nullptr,              // file (optional)
            0,                    // line number
            8,                    // size in bits
            8,                    // alignment in bits
            llvm::DINode::FlagZero,
            nullptr,              // derived from
            builder.getOrCreateArray({elementType}) // members
        );

        return structType;
    }

    uint64_t CharType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 8;
    }

    uint32_t CharType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 8;
    }

    llvm::DINode::DIFlags CharType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}
