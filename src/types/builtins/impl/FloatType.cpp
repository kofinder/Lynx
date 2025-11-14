#include "builtins/FloatType.hpp"
#include <context/AstContext.hpp>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/FloatMethodResolver.hpp"

using namespace LynxContext;


namespace LynxTypes {

    llvm::Type* FloatType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return llvm::Type::getFloatTy(astContext->getLLVMContext());
    }

    llvm::Type* FloatType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto* floatTy = llvm::Type::getFloatTy(astContext->getLLVMContext());
        auto* floatPtr = llvm::PointerType::get(floatTy->getContext(), 0); 
        return floatPtr;
    }

    llvm::Value* FloatType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0.0f;
        return this->createValue(LValueType);
    }

    llvm::Value* FloatType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* floatType = this->getLLVMType();
        auto var = builder.CreateAlloca(floatType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::floatType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        
        return var;
    }

    llvm::Value* FloatType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if(std::holds_alternative<float>(value)) {
            auto& context = astContext->getLLVMContext();
            float floatValue = std::get<float>(value);
            return llvm::ConstantFP::get(context, llvm::APFloat(floatValue));
        } 
        LOG_ERROR("Unsupported value type");
        return nullptr;
    }

    llvm::Value* FloatType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void FloatType::accept(TypeVisitor& visitor) { 
        LOG_INFO("Invoked...");
        visitor.visit(*this); 
    }

    const std::unordered_map<std::string, int>& FloatType::getMethodRegistry() const {
        const static std::unordered_map<std::string, int> methodTypes = {
            {"max", 0}, 
            {"min", 0}, 
            {"fromString", 1} 
        };
        return methodTypes;
    }

    TypeMethodResolver* FloatType::getOrCreateResolver() const { 
        if (!resolver) {
            resolver = new FloatMethodResolver();
        }
        return resolver;
    }

    llvm::Value* FloatType::emitMethodCall(llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
        return nullptr;
    }

    const BaseType* FloatType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* FloatType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool FloatType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const FloatType*>(other) != nullptr;
    }

    std::string FloatType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "float";
    }

    llvm::DIType* FloatType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();
        return builder.createBasicType(
            getDebugName(),        // "float"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_float
        );
    }

    uint64_t FloatType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 32; // 32-bit IEEE 754 float
    }

    uint32_t FloatType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32; // Alignment typically matches the size for primitive types
    }

    llvm::DINode::DIFlags FloatType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}