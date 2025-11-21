#include "sequential/VectorType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {

    llvm::Type* VectorType::computeLLVMType() const {
        llvm::Type* elemLLVMType = elementType->getLLVMType();
        if (!elemLLVMType) {
            LOG_ERROR("Element type is null.");
            return nullptr;
        }

        if (llvm::isa<llvm::VectorType>(elemLLVMType) || llvm::isa<llvm::StructType>(elemLLVMType)) {
            std::string nestedName = getSafeStructName();
            llvm::StructType* existing = llvm::StructType::getTypeByName(astContext->getLLVMContext(), nestedName);
            if (existing) {
                cachedLLVMType = existing;
                return existing;
            }
            std::vector<llvm::Type*> members(numElements, elemLLVMType);
            cachedLLVMType = llvm::StructType::create(astContext->getLLVMContext(), members, nestedName);
            return cachedLLVMType;
        }
        
        llvm::ElementCount ec = llvm::ElementCount::getFixed(numElements);
        cachedLLVMType = llvm::VectorType::get(elemLLVMType, ec);
        return cachedLLVMType;
    }    
    
    llvm::Type* VectorType::getLLVMPointerType() const {
        return computeLLVMType();
    }

    llvm::Value* VectorType::getDefaultValue() {
        llvm::Type* vecType = computeLLVMType();
        return llvm::Constant::getNullValue(vecType);    
    }

    llvm::Value* VectorType::createInstance(std::string variableName) {
        llvm::Type* vectorType = this->computeLLVMType();
        auto& builder = astContext->getBuilder();        
        auto* var = builder.CreateAlloca(vectorType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::vectorType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        var->setAlignment(llvm::Align(32));
        return var;
    }

    llvm::Value* VectorType::createValue(std::vector<llvm::Value*> values) const {

        llvm::Type* elemType = elementType->getLLVMType();
        llvm::Type* computedType = computeLLVMType();
    
        if (values.empty()) {
            return llvm::UndefValue::get(computedType);
        }
    
        bool allConstant = std::all_of(values.begin(), values.end(), [](llvm::Value* v) {
            return llvm::isa<llvm::Constant>(v);
        });
    
        if (auto* vecTy = llvm::dyn_cast<llvm::VectorType>(computedType)) {
            if (allConstant) {
                llvm::SmallVector<llvm::Constant*, 8> constants;
                for (llvm::Value* v : values) {
                    constants.push_back(llvm::cast<llvm::Constant>(v));
                }
                return llvm::ConstantVector::get(constants);
            } else {
                llvm::Value* vec = llvm::UndefValue::get(vecTy);
                auto& builder = astContext->getBuilder();
                for (unsigned i = 0; i < values.size(); ++i) {
                    vec = builder.CreateInsertElement(vec, values[i], builder.getInt32(i));
                }
                return vec;
            }
        }
    
        if (auto* structTy = llvm::dyn_cast<llvm::StructType>(computedType)) {
            if (!allConstant) {
                LOG_ERROR("Nested vector init must be constant");
                return nullptr;
            }
            llvm::SmallVector<llvm::Constant*, 8> nestedConstants;
            for (llvm::Value* val : values) {
                nestedConstants.push_back(llvm::cast<llvm::Constant>(val));
            }
            return llvm::ConstantStruct::get(structTy, nestedConstants);
        }
    
        LOG_ERROR("Unhandled LLVM type in VectorType::createValue()");
        return nullptr;
    }
    
    llvm::Value* VectorType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    bool VectorType::equals(const BaseType* other) const {
        return false;
    }

    std::string VectorType::getSafeStructName(std::unordered_set<const BaseType*>& visited) const {
        if (visited.find(this) != visited.end()) {
            return "vec_recursive";
        }
        visited.insert(this);
    
        if (auto nested = dynamic_cast<const VectorType*>(elementType)) {
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