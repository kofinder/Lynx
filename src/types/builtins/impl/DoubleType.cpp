#include "builtins/DoubleType.hpp"
#include <context/AstContext.hpp>
#include <resolver/TypeVisitor.hpp>
#include <resolver/TypeMethodResolver.hpp>
#include <resolver/DoubleMethodResolver.hpp>

using namespace LynxContext;

namespace LynxTypes {

    llvm::Type* DoubleType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getDoubleTy(context);  // Correct type for 64-bit double
    }

    llvm::Type* DoubleType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getDoublePtrTy(context);  // Pointer to double type
    }

    llvm::Value* DoubleType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0.0;
        return this->createValue(LValueType);
    }

    llvm::Value* DoubleType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* doubleType = this->getLLVMType();
        auto var = builder.CreateAlloca(doubleType, nullptr, variableName);

        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::doubleType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DoubleType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if(std::holds_alternative<double>(value)) {
            auto& context = astContext->getLLVMContext();
            double doubleValue = std::get<double>(value);
            return llvm::ConstantFP::get(context, llvm::APFloat(doubleValue));
        }

        LOG_ERROR("Unsupported value type!");
        return nullptr;
    }

    llvm::Value* DoubleType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void DoubleType::accept(TypeVisitor& visitor) { 
        LOG_INFO("Invoked...");
        visitor.visit(*this); 
    }

    std::unique_ptr<TypeMethodResolver> DoubleType::createMethodResolver() const {
        LOG_INFO("Invoked...");
        return std::make_unique<DoubleMethodResolver>();
    }

    const BaseType* DoubleType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* DoubleType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool DoubleType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const DoubleType*>(other) != nullptr;
    }

    std::string DoubleType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "Double";
    }

    llvm::DIType* DoubleType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        llvm::DIFile* file = builder.createFile("double.lynx", ".");

        return builder.createBasicType(
            getDebugName(),                  // "Double"
            getDebugSizeInBits(),            // 64 bits
            llvm::dwarf::DW_ATE_float        // Floating-point type
        );
    }

    uint64_t DoubleType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 64; // 64-bit IEEE 754
    }

    uint32_t DoubleType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 64; // Align to 64 bits
    }

    llvm::DINode::DIFlags DoubleType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}
