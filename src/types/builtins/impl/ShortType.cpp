#include "builtins/ShortType.hpp"
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/ShortMethodResolver.hpp"
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes {

    using namespace LynxConstants;


    llvm::Type* ShortType::computeLLVMType() const {
        return llvm::Type::getInt16Ty(getLLVMContext());
    }

    llvm::Type* ShortType::getLLVMPointerType() const {
        auto* shortTy = llvm::Type::getInt16Ty(getLLVMContext());
        return llvm::PointerType::get(shortTy->getContext(), 0);
    }

    llvm::Value* ShortType::getDefaultValue() {
        const LValueType value = 0;
        return createValue(value);
    }

    llvm::Value* ShortType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(computeLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::shortType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }

        return var;
    }

    llvm::Value* ShortType::createValue(LValueType value) const {
        if(std::holds_alternative<short>(value)) {
            auto& context = getLLVMContext();
            const short shortValue = std::get<short>(value);
            return llvm::ConstantInt::get(context, llvm::APInt(BIT_WIDTH_SHORT, shortValue));
        }

        std::cerr << "Unsupported value type!\n";
        return nullptr;
    }

    llvm::Value* ShortType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return getBuilder().CreateStore(rhs, lhs);
    }
    
    void ShortType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* ShortType::getOrCreateResolver() const { return ShortMethodResolver::create(); }

    llvm::Value* ShortType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        auto* resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
    }

    bool ShortType::equals(const BaseType* other) const {
        return dynamic_cast<const ShortType*>(other) != nullptr;
    }

    const BaseType* ShortType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ShortType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ShortType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ShortType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ShortType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ShortType::getDIFlags() const { return llvm::DINode::FlagZero; }
}