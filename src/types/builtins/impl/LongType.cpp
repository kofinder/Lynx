#include "builtins/LongType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/LongMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* LongType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return llvm::Type::getInt64Ty(astContext->getLLVMContext());
    }

    llvm::Type* LongType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto* longTy = llvm::Type::getInt64Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(longTy->getContext(), 0);
    }

    llvm::Value* LongType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0;
        return this->createValue(LValueType);
    }

    llvm::Value* LongType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* longType = this->getLLVMType();
        auto var = builder.CreateAlloca(longType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::longType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* LongType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if(std::holds_alternative<long>(value)) {
            auto& context = astContext->getLLVMContext();
            long longValue = std::get<long>(value);
            return llvm::ConstantInt::get(context, llvm::APInt(64, longValue));
        }

        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* LongType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void LongType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    llvm::Value* LongType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver)  resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    TypeMethodResolver* LongType::getOrCreateResolver() const { 
        if (!resolver) {
            resolver = new LongMethodResolver();
        }
        return resolver;
    }

    const BaseType* LongType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* LongType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool LongType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const LongType*>(other) != nullptr;
    }

    std::string LongType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "long";
    }

    llvm::DIType* LongType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),         // "long"
            getDebugSizeInBits(),   // 64 bits
            llvm::dwarf::DW_ATE_signed
        );
    }

    uint64_t LongType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 64; // 64-bit signed integer
    }

    uint32_t LongType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 64; // Common alignment for 64-bit integers
    }

    llvm::DINode::DIFlags LongType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}