#include "infrerence/AutoType.hpp"
#include <resolver/TypeVisitor.hpp>
#include <resolver/methods/AutoMethodResolver.hpp>

namespace LynxTypes {

    llvm::Type* AutoType::computeLLVMType() const {
        LOG_INFO("Invoked...");
        if (!inferredType) {
            LOG_ERROR("Attempting to use 'auto' type before inference.");
            return nullptr;
        }
        return inferredType->getLLVMType();
    }

    const BaseType* AutoType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        if (this->isStatic() == newIsStatic) return this;
        return new AutoType(astContext);
    }

    const BaseType* AutoType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        if (this->isConst() == newIsConst) return this;
        return new AutoType(astContext);
    }

    void AutoType::accept(TypeVisitor& visitor) {
        LOG_INFO("Invoked...");
       visitor.visit(*this);
    }

    llvm::Value* AutoType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->createInstance(variableName);
    }

    llvm::Value* AutoType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->assignTo(lhs, rhs);
    }

    std::unique_ptr<TypeMethodResolver> AutoType::createMethodResolver() const {
        LOG_INFO("Invoked...");
        return std::make_unique<AutoMethodResolver>();
    }

    llvm::Value* AutoType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->createValue(value);
    }

    llvm::Value* AutoType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked...");
        astContext->reportError(makeRuntimeError("AutoType createValue(T, V) doesn't support this createValue signature."));
        return nullptr;    
    }  
    
    llvm::Value* AutoType::createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const {
        LOG_INFO("Invoked...");
        astContext->reportError(makeRuntimeError("AutoType createValue(T, K, V) doesn't support this createValue signature."));
        return nullptr;    
    }    

    llvm::Type* AutoType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getLLVMPointerType();
    }

    llvm::Value* AutoType::getDefaultValue() {
        LOG_INFO("Invoked...");
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getDefaultValue();
    }

    bool AutoType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        if (auto* otherAuto = dynamic_cast<const AutoType*>(other)) {
            if (isInferred() && otherAuto->isInferred()) {
                return inferredType->equals(otherAuto->inferredType);
            }
            return !isInferred() && !otherAuto->isInferred();
        }
        return false;
    }

    bool AutoType::canAccept(const BaseType* other) const {
        return equals(other);
    }


    std::string AutoType::getDebugName() const {
        if (!inferredType) return "auto";
        return "auto(" + inferredType->getDebugName() + ")";
    }

    llvm::DIType* AutoType::getDIType(llvm::DIScope* scope) const {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getDIType(scope);
    }

    uint64_t AutoType::getDebugSizeInBits() const {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getDebugSizeInBits();
    }

    uint32_t AutoType::getDebugAlignInBits() const {
        if (!inferredType) throw std::logic_error("AutoType not yet inferred.");
        return inferredType->getDebugAlignInBits();
    }

    llvm::DINode::DIFlags AutoType::getDIFlags() const {
        llvm::DINode::DIFlags flags = llvm::DINode::FlagZero;

        if (isConst()) {
            flags |= llvm::DINode::FlagArtificial;
        }

        if (isStatic()) {
            flags |= llvm::DINode::FlagStaticMember;
        }

        return flags;
    }
}