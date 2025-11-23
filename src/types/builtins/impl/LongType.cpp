#include "builtins/LongType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/LongMethodResolver.hpp"
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes {

    using namespace LynxConstants;

    llvm::Type* LongType::computeLLVMType() const {
        return llvm::Type::getInt64Ty(astContext->getLLVMContext());
    }

    llvm::Type* LongType::getLLVMPointerType() const {
        auto* longTy = llvm::Type::getInt64Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(longTy->getContext(), 0);
    }

    llvm::Value* LongType::getDefaultValue() {
        const LValueType value = 0;
        return createValue(value);
    }

    llvm::Value* LongType::createInstance(const std::string& variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* longType = this->getLLVMType();
        auto* var = builder.CreateAlloca(longType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::longType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* LongType::createValue(LValueType value) const {
        if(std::holds_alternative<long>(value)) {
            auto& context = astContext->getLLVMContext();
            const long longValue = std::get<long>(value);
            return llvm::ConstantInt::get(context, llvm::APInt(BIT_WIDTH_LONG, longValue));
        }

        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* LongType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        // Perform the store operation: store rhs value into lhs
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void LongType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* LongType::getOrCreateResolver() const { return LongMethodResolver::create(); }

    llvm::Value* LongType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver)  resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    bool LongType::equals(const BaseType* other) const {
        return dynamic_cast<const LongType*>(other) != nullptr;
    }

    const BaseType* LongType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* LongType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* LongType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t LongType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t LongType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags LongType::getDIFlags() const { return llvm::DINode::FlagZero; }
}