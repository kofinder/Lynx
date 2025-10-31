#include "builtins/BooleanType.hpp"
#include <context/AstContext.hpp>
#include <resolver/TypeVisitor.hpp>
#include <resolver/TypeMethodResolver.hpp>
#include <resolver/BoolMethodResolver.hpp>

using namespace LynxContext;

namespace LynxTypes {

    llvm::Type* BooleanType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getInt1Ty(context);
    }

    llvm::Type* BooleanType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        return llvm::Type::getInt1PtrTy(context);
    }

    llvm::Value* BooleanType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = false;
        return this->createValue(LValueType);
    }


    llvm::Value* BooleanType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* booType = this->getLLVMType();
        auto var = builder.CreateAlloca(booType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::booleanType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* BooleanType::convertBooleanToString(llvm::Value* value) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        // Directly use the boolean (i1) to select the string value.
        llvm::Value* booleanAsString = builder.CreateSelect(
            value,  // Boolean condition (i1)
            builder.CreateGlobalStringPtr("true"),  // If true
            builder.CreateGlobalStringPtr("false")  // If false
        );
        
        return booleanAsString;
    }

    llvm::Value* BooleanType::createValue(LValueType value) const {
        LOG_ERROR("Invoked...");
        if(std::holds_alternative<bool>(value)) {
            bool boolValue = std::get<bool>(value);
            auto& context = astContext->getLLVMContext();
            return llvm::ConstantInt::get(context, llvm::APInt(1, boolValue));
        }
        LOG_ERROR("BooleanType: Unsupported value type!");
        return nullptr;
    }

    llvm::Value* BooleanType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void BooleanType::accept(TypeVisitor& visitor) { 
        LOG_INFO("Invoked...");
        visitor.visit(*this); 
    }

    std::unique_ptr<TypeMethodResolver> BooleanType::createMethodResolver() const { 
        LOG_INFO("Invoked...");
        return std::make_unique<BoolMethodResolver>();
    }

    const BaseType* BooleanType::createWithStatic(bool newIsStatic) const {
        return nullptr;
    }

    const BaseType* BooleanType::createWithConst(bool newIsConst) const {
        return nullptr;
    }

    bool BooleanType::equals(const BaseType* other) const {
        return dynamic_cast<const BooleanType*>(other) != nullptr;
    }

    std::string BooleanType::getDebugName() const { 
        LOG_INFO("Invoked...");
        return "bool"; 
    }

    llvm::DIType* BooleanType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        return builder.createBasicType("bool", 1, llvm::dwarf::DW_ATE_boolean);
    }

    uint64_t BooleanType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 1;
    }

    uint32_t BooleanType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 1;
    }

    llvm::DINode::DIFlags BooleanType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }

}