#include "builtins/ShortType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/ShortMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* ShortType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return llvm::Type::getInt16Ty(astContext->getLLVMContext());
    }

    llvm::Type* ShortType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto* shortTy = llvm::Type::getInt16Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(shortTy->getContext(), 0);
    }

    llvm::Value* ShortType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0;
        return this->createValue(LValueType);
    }

    llvm::Value* ShortType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* shortType = this->getLLVMType();
        auto var = builder.CreateAlloca(shortType, nullptr, variableName);

        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::shortType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* ShortType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if(std::holds_alternative<short>(value)) {
            auto& context = astContext->getLLVMContext();
            short shortValue = std::get<short>(value);
            return llvm::ConstantInt::get(context, llvm::APInt(16, shortValue));
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }

    llvm::Value* ShortType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void ShortType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    llvm::Value* ShortType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    TypeMethodResolver* ShortType::getOrCreateResolver() const { 
        if (!resolver) resolver = new ShortMethodResolver();
        return resolver;
    }

    const BaseType* ShortType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* ShortType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool ShortType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const ShortType*>(other) != nullptr;
    }

    std::string ShortType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "short";
    }

    llvm::DIType* ShortType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),         // "short"
            getDebugSizeInBits(),   // 16 bits
            llvm::dwarf::DW_ATE_signed
        );
    }

    uint64_t ShortType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 16; // 16-bit integer
    }

    uint32_t ShortType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 16; // Typically 16-bit alignment
    }

    llvm::DINode::DIFlags ShortType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}