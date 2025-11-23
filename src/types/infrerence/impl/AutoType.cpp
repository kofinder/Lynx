#include "infrerence/AutoType.hpp"
#include "visitor/TypeVisitor.hpp"
#include <resolver/methods/AutoMethodResolver.hpp>

namespace LynxTypes {

    llvm::Type* AutoType::computeLLVMType() const {
        if (!inferredType) {
            LOG_ERROR("Attempting to use 'auto' type before inference.");
            return nullptr;
        }
        return inferredType->getLLVMType();
    }

    llvm::Value* AutoType::createInstance(const std::string& variableName) {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->createInstance(variableName);
    }

    llvm::Value* AutoType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->assignTo(lhs, rhs);
    }

    llvm::Value* AutoType::createValue(LValueType value) const {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->createValue(value);
    }

    llvm::Value* AutoType::createValue(std::vector<llvm::Value*> /*values*/) const {
        astContext->reportError(makeRuntimeError("AutoType createValue(T, V) doesn't support this createValue signature."));
        return nullptr;    
    }  
    
    llvm::Value* AutoType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*pairs*/) const {
        astContext->reportError(makeRuntimeError("AutoType createValue(T, K, V) doesn't support this createValue signature."));
        return nullptr;    
    }    

    llvm::Type* AutoType::getLLVMPointerType() const {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getLLVMPointerType();
    }

    llvm::Value* AutoType::getDefaultValue() {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getDefaultValue();
    }

    bool AutoType::equals(const BaseType* other) const {
        if (const auto* otherAuto = dynamic_cast<const AutoType*>(other)) {
            if (isInferred() && otherAuto->isInferred()) return inferredType->equals(otherAuto->inferredType);
            return !isInferred() && !otherAuto->isInferred();
        }
        return false;
    }

    bool AutoType::canAccept(const BaseType* other) const { return equals(other); }

    const BaseType* AutoType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* AutoType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* AutoType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t AutoType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t AutoType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags AutoType::getDIFlags() const { return llvm::DINode::FlagZero; }
}