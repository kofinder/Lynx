#include "builtins/DoubleType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/DoubleMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* DoubleType::computeLLVMType() const {
        return llvm::Type::getDoubleTy(getLLVMContext());
    }

    llvm::Type* DoubleType::getLLVMPointerType() const {
        auto* doubleTy = llvm::Type::getDoubleTy(getLLVMContext());
        return llvm::PointerType::get(doubleTy->getContext(), 0);
    }

    llvm::Value* DoubleType::getDefaultValue() {
        const LValueType value = 0.0;
        return createValue(value);
    }

    llvm::Value* DoubleType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::doubleType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DoubleType::createValue(LValueType value) const {
        if(std::holds_alternative<double>(value)) {
            auto& context = getLLVMContext();
            const double doubleValue = std::get<double>(value);
            return llvm::ConstantFP::get(context, llvm::APFloat(doubleValue));
        }

        std::cerr << "Unsupported value type!\n";
        return nullptr;
    }

    llvm::Value* DoubleType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {  
        return getBuilder().CreateStore(rhs, lhs);
    }

    void DoubleType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* DoubleType::getOrCreateResolver() const { return DoubleMethodResolver::create(); }

    llvm::Value* DoubleType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        auto* resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
    }

    bool DoubleType::equals(const BaseType* other) const {
        return dynamic_cast<const DoubleType*>(other) != nullptr;
    }

    const BaseType* DoubleType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* DoubleType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* DoubleType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t DoubleType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t DoubleType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags DoubleType::getDIFlags() const { return llvm::DINode::FlagZero; }
}
