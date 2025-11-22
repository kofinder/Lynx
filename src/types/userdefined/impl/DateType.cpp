#include "userdefined/DateType.hpp"
#include <context/AstContext.hpp>


namespace LynxTypes {

    llvm::StructType* DateType::cachedType = nullptr;

    llvm::Type* DateType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureDateType);
            const std::vector<llvm::Type*> members = {
                llvm::Type::getInt32Ty(context), // year
                llvm::Type::getInt32Ty(context), // month
                llvm::Type::getInt32Ty(context), // day
                llvm::Type::getInt32Ty(context), // hour
                llvm::Type::getInt32Ty(context), // minute
                llvm::Type::getInt32Ty(context), // second
                llvm::Type::getInt32Ty(context)  // millisecond
            };
            cachedType = llvm::StructType::create(members, MetadataTypeConstants::structureDateType);
        }

        return cachedType;
    }

    llvm::Type* DateType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* DateType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());
    }

    llvm::Value* DateType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* doubleType = this->getLLVMType();
        auto* var = builder.CreateAlloca(doubleType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::dateType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DateType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* DateType::getField(const std::string& /*fieldName*/, llvm::Value* instance) {
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        llvm::Function* func = nullptr;
        return builder.CreateCall(func, { instance });    
    }

    bool DateType::equals(const BaseType* other) const {
        return dynamic_cast<const DateType*>(other) != nullptr;
    }
    
    const BaseType* DateType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* DateType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* DateType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t DateType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t DateType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags DateType::getDIFlags() const { return llvm::DINode::FlagZero; }
}