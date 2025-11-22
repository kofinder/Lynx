#include "sequential/VectorType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* VectorType::computeLLVMType() const {
        llvm::Type* elemLLVMType = elementType->getLLVMType();
        if (!elemLLVMType)  return nullptr;
        if (llvm::isa<llvm::VectorType>(elemLLVMType) || llvm::isa<llvm::StructType>(elemLLVMType)) {
            const auto nestedName = getSafeStructName();
            auto* existing = llvm::StructType::getTypeByName(astContext->getLLVMContext(), nestedName);
            if (existing) {
                cachedLLVMType = existing;
                return existing;
            }
            const std::vector<llvm::Type*> members(numElements, elemLLVMType);
            cachedLLVMType = llvm::StructType::create(astContext->getLLVMContext(), members, nestedName);
            return cachedLLVMType;
        }
        
        const auto eleCount = llvm::ElementCount::getFixed(numElements);
        cachedLLVMType = llvm::VectorType::get(elemLLVMType, eleCount);
        return cachedLLVMType;
    }    
    
    llvm::Type* VectorType::getLLVMPointerType() const {
        return computeLLVMType();
    }

    llvm::Value* VectorType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());    
    }

    llvm::Value* VectorType::createInstance(std::string variableName) {
        llvm::Type* vectorType = computeLLVMType();
        auto& builder = astContext->getBuilder();        
        auto* var = builder.CreateAlloca(vectorType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::vectorType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        var->setAlignment(llvm::Align(vectorDefaultAlignSize));
        return var;
    }

    llvm::Value* VectorType::createConstantStructValue(llvm::StructType* structTy, const std::vector<llvm::Value*>& values) const {
        llvm::SmallVector<llvm::Constant*, kSmallVectorInitialSize> consts;
        for (auto* val : values) {
            if (auto* elem = llvm::dyn_cast<llvm::Constant>(val)) {
                consts.push_back(elem);
            } else {
                return llvm::UndefValue::get(structTy);
            }
        }
        return llvm::ConstantStruct::get(structTy, consts);
    }
    
    llvm::Value* VectorType::createNonConstantStructValue(llvm::StructType* structTy, const std::vector<llvm::Value*>& values) const {
        auto& builder = astContext->getBuilder();
        llvm::Value* aggregate = llvm::UndefValue::get(structTy);
        for (unsigned i = 0; i < values.size(); ++i) {
            aggregate = builder.CreateInsertValue(aggregate, values[i], {i});
        }
        return aggregate;
    }
    

    llvm::Value* VectorType::createValue(const std::vector<llvm::Value*> values) const {
        auto* computedType = computeLLVMType();
        if (!computedType) return nullptr;
        if (values.empty()) return llvm::UndefValue::get(computedType);
    
        const bool allConstants = std::ranges::all_of(values, [](llvm::Value* value) {
            return llvm::isa<llvm::Constant>(value);
        });
    
        if (auto* outerStruct = llvm::dyn_cast<llvm::StructType>(computedType)) {
            if (allConstants) return createConstantStructValue(outerStruct, values);
            return createNonConstantStructValue(outerStruct, values);
        }
    
        return llvm::UndefValue::get(computedType);
    }
    
    llvm::Value* VectorType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool VectorType::equals(const BaseType* other) const {
        if (other->getTypeTag() != DataType::VECTOR) return false;
        const auto* otherVector = dynamic_cast<const VectorType*>(other);
        return otherVector && numElements == otherVector->numElements && elementType->equals(otherVector->elementType);
    }

    // NOLINTNEXTLINE(misc-no-recursion)
    std::string VectorType::getSafeStructName(std::unordered_set<const BaseType*>& visited) const {
        if (visited.contains(this)) return "vec_recursive";
        
        visited.insert(this);
    
        if (!elementType) return "vec_unknown_" + std::to_string(numElements);
    
        if (const auto* nested = dynamic_cast<const VectorType*>(elementType)) {
            return "vec_nested_" + std::to_string(numElements) + "_of_" + nested->getSafeStructName(visited);
        }
    
        return "vec_of_" + elementType->getDebugName() + "_" + std::to_string(numElements);
    }

    std::string VectorType::getSafeStructName() const {
        std::unordered_set<const BaseType*> visited;
        return getSafeStructName(visited);    
    }

    const BaseType* VectorType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* VectorType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* VectorType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t VectorType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t VectorType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags VectorType::getDIFlags() const { return llvm::DINode::FlagZero; }
}
