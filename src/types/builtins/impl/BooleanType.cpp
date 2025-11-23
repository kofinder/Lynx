#include "builtins/BooleanType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/BoolMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* BooleanType::computeLLVMType() const {
        return llvm::Type::getInt1Ty(getLLVMContext());
    }

    llvm::Type* BooleanType::getLLVMPointerType() const {
        auto* boolTy = llvm::Type::getInt1Ty(getLLVMContext());
        return llvm::PointerType::get(boolTy->getContext(), 0);
    }

    llvm::Value* BooleanType::getDefaultValue() {
        const LValueType value = false;
        return createValue(value);
    }

    llvm::Value* BooleanType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::booleanType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    const llvm::Value* BooleanType::convertBooleanToString(llvm::Value* value) {
        auto& builder = getBuilder();
        const auto* booleanAsString = builder.CreateSelect(value, builder.CreateGlobalString("true"), builder.CreateGlobalString("false"));
        return booleanAsString;
    }

    llvm::Value* BooleanType::createValue(LValueType value) const {
        if(std::holds_alternative<bool>(value)) {
            const bool boolValue = std::get<bool>(value);
            return llvm::ConstantInt::get(getLLVMContext(), llvm::APInt(1, static_cast<uint64_t>(boolValue)));
        }
        return nullptr;
    }

    llvm::Value* BooleanType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return getBuilder().CreateStore(rhs, lhs);
    }

    void BooleanType::accept(TypeVisitor& visitor) { visitor.visit(*this); }
    
    TypeMethodResolver* BooleanType::getOrCreateResolver() const { return nullptr; }

    llvm::Value* BooleanType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        auto* resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
    }

    bool BooleanType::equals(const BaseType* other) const {
        return dynamic_cast<const BooleanType*>(other) != nullptr;
    }

    const BaseType* BooleanType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* BooleanType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* BooleanType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t BooleanType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t BooleanType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags BooleanType::getDIFlags() const { return llvm::DINode::FlagZero; }
}