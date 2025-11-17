#include "builtins/IntegerType.hpp"
#include <llvm/IR/DebugInfoMetadata.h>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/IntMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* IntegerType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        return llvm::Type::getInt32Ty(astContext->getLLVMContext());
    }

    llvm::Type* IntegerType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        auto* intTy = llvm::Type::getInt32Ty(astContext->getLLVMContext());
        return llvm::PointerType::get(intTy->getContext(), 0);
    }

    llvm::Value* IntegerType::getDefaultValue() {
        LOG_INFO("Invoked...");
        LValueType LValueType = 0;
        return this->createValue(LValueType);
    }

    llvm::Value* IntegerType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* intType = this->getLLVMType();
        auto var = builder.CreateAlloca(intType, nullptr, variableName); 

        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::intType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }       
       
        return var;
    }

    llvm::Value* IntegerType::createValue(LValueType value) const {
        if(std::holds_alternative<int>(value)) {
            auto& context = astContext->getLLVMContext();
            int intValue = std::get<int>(value); 
            return llvm::ConstantInt::get(context, llvm::APInt(32, intValue));
        }

        LOG_ERROR("Unsupported value type");
        return nullptr;
    }
 
    llvm::Value* IntegerType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }
    
    void IntegerType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    TypeMethodResolver* IntegerType::getOrCreateResolver() const { return IntMethodResolver::create(); }

    llvm::Value* IntegerType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver) resolver = IntMethodResolver::create();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    const BaseType* IntegerType::createWithStatic(bool newIsStatic) const {
        auto clone = std::make_shared<IntegerType>(astContext);
        clone->createWithStatic(newIsStatic);
        clone->createWithConst(isConst());
        return clone.get();
    }

    const BaseType* IntegerType::createWithConst(bool newIsConst) const {
        auto clone = std::make_shared<IntegerType>(astContext);
        clone->setConst(newIsConst);
        clone->setStatic(isStatic());
        return clone.get();
    }
    
    bool IntegerType::equals(const BaseType* other) const {
        auto* otherInteger = dynamic_cast<const IntegerType*>(other);
        if (!otherInteger) return false;
        return this->isConst() == otherInteger->isConst() &&
               this->isStatic() == otherInteger->isStatic();
    }
    

    std::string IntegerType::getDebugName() const {
        LOG_INFO("Invoked...");
        return isConst() ? "const int" : "int";
    }

    llvm::DIType* IntegerType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        return builder.createBasicType(
            getDebugName(),        // "int"
            getDebugSizeInBits(),  // 32 bits
            llvm::dwarf::DW_ATE_signed
        );
    }

    uint64_t IntegerType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 32; // 32-bit signed integer
    }

    uint32_t IntegerType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32; // Common alignment for 32-bit integers
    }
    
    llvm::DINode::DIFlags IntegerType::getDIFlags() const {
        LOG_INFO("Invoked...");
        llvm::DINode::DIFlags flags = llvm::DINode::FlagZero;
    
        if (isStatic()) {
            flags |= llvm::DINode::FlagStaticMember;
        }
        return flags;
    }
    
}

