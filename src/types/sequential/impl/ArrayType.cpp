#include <types/sequential/ArrayType.hpp>
#include <context/AstContext.hpp>
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes {

    using namespace LynxContext;
    using namespace LynxTypes;
    using namespace LynxConstants;

    llvm::Type* ArrayType::computeLLVMType() const {

        if(!elementType) return nullptr;
    
        auto* elemLLVMType = elementType->getLLVMType();
        if (!elemLLVMType)  return nullptr;
        if (numElements <= 0) return nullptr;
        
        auto& context = astContext->getLLVMContext();
    
        if (llvm::isa<llvm::StructType>(elemLLVMType)) {
            auto* innerStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::innerArray);
            if (!innerStruct) innerStruct = llvm::StructType::create(context, elemLLVMType, MetadataTypeConstants::innerArray);
    
            auto* outerArrayTy = llvm::ArrayType::get(innerStruct, numElements);
            auto* outerStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::outerArray);
            if (!outerStruct)  outerStruct = llvm::StructType::create(context, outerArrayTy, MetadataTypeConstants::outerArray);
    
            cachedLLVMType = outerStruct;
            return cachedLLVMType;
        }
    
        auto* arrayType = llvm::ArrayType::get(elemLLVMType, numElements);
        auto* arrayStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::simpleArray);
        if (!arrayStruct) arrayStruct = llvm::StructType::create(context, arrayType, MetadataTypeConstants::simpleArray);
    
        cachedLLVMType = arrayStruct;
        return cachedLLVMType;
    }

    llvm::Value* ArrayType::createConstantStructValue(llvm::StructType* structTy, const std::vector<llvm::Value*>& values) const {
         auto* arrayTy = llvm::cast<llvm::ArrayType>(structTy->getElementType(0)); 
         auto* innerTy = arrayTy->getElementType(); 
         if (auto* innerStruct = llvm::dyn_cast<llvm::StructType>(innerTy)) {
            llvm::SmallVector<llvm::Constant*, SMALL_VECTOR_INITIAL_SIZE> constElems; 
            for (auto* val : values) { 
                constElems.push_back(llvm::cast<llvm::Constant>(val)); 
            } 
            auto* constArray = llvm::ConstantArray::get(arrayTy, constElems); 
            return llvm::ConstantStruct::get(structTy, constArray); 
        } 

        llvm::SmallVector<llvm::Constant*, SMALL_VECTOR_INITIAL_SIZE> constElems; 
        for (auto* val : values) { 
            constElems.push_back(llvm::cast<llvm::Constant>(val));
        } 
        auto* constArray = llvm::ConstantArray::get(arrayTy, constElems); 
        return llvm::ConstantStruct::get(structTy, constArray);
    }

    llvm::Value* ArrayType::createNonConstantStructValue(llvm::StructType* structTy, const std::vector<llvm::Value*>& values) const {
        auto& builder = getContext()->getBuilder();
        auto* undefStruct = llvm::UndefValue::get(structTy); 
        auto* undefArray = builder.CreateExtractValue(undefStruct, {0}); 
        for (unsigned i = 0; i < values.size(); ++i) { 
            undefArray = builder.CreateInsertValue(undefArray, values[i], {i}); 
        } 
        return builder.CreateInsertValue(undefStruct, undefArray, {0});
    }

    llvm::Value* ArrayType::createValue(const std::vector<llvm::Value*> values) const {
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

    llvm::Value* ArrayType::createInstance(const std::string& variableName) {
        auto& builder = getContext()->getBuilder();
        llvm::Type* llvmType = computeLLVMType();
        auto* var = builder.CreateAlloca(llvmType, nullptr, variableName); 
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::arrayType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }       
        return var;
    }

    llvm::Type* ArrayType::getLLVMPointerType() const {
        auto* arrayType = computeLLVMType();
        return llvm::PointerType::get(arrayType->getContext(), 0);
    }

    llvm::Value* ArrayType::getDefaultValue() {
        return llvm::ConstantAggregateZero::get(computeLLVMType());
    }

    llvm::Value* ArrayType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        auto& builder = getContext()->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* ArrayType::getElementPointer(llvm::Value* arrayAlloca, int index) const {
        auto& builder = getContext()->getBuilder();
        auto& context = getContext()->getLLVMContext();
        llvm::Type* structType = computeLLVMType();

        if (!llvm::isa<llvm::StructType>(structType)) return nullptr;
    
        auto* arrayPtr = builder.CreateStructGEP(structType, arrayAlloca, 0, "array_gep");
        auto* innerType = llvm::cast<llvm::StructType>(structType)->getElementType(0);
        if (auto* outerArrayTy = llvm::dyn_cast<llvm::ArrayType>(innerType)) {
            auto* elementTy = outerArrayTy->getElementType();
            if (auto* innerStruct = llvm::dyn_cast<llvm::StructType>(elementTy)) {
                auto* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
                auto* idx = llvm::ConstantInt::get(builder.getInt32Ty(), index);
                auto* structElemPtr = builder.CreateGEP(outerArrayTy, arrayPtr, {zero, idx}, "outer_elem_ptr");
                auto* innerArrayPtr = builder.CreateStructGEP(innerStruct, structElemPtr, 0, "inner_array_ptr");
                return innerArrayPtr;
            }
        }
    
        auto* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
        auto* idx = llvm::ConstantInt::get(builder.getInt32Ty(), index);
        return builder.CreateGEP(innerType, arrayPtr, { zero, idx }, "elem_ptr");
    }

    llvm::Value* ArrayType::getElementPointer(llvm::Value* arrayAlloca, int outerIndex, int innerIndex) const {
        auto& builder = getContext()->getBuilder();
        auto& context = getContext()->getLLVMContext();
    
        auto* outerStructTy = computeLLVMType();
        if (!llvm::isa<llvm::StructType>(outerStructTy)) return nullptr;
    
        auto* outerArrayPtr = builder.CreateStructGEP(outerStructTy, arrayAlloca, 0, "outer_struct.array");
        auto* outerArrayTy = llvm::dyn_cast<llvm::ArrayType>(llvm::cast<llvm::StructType>(outerStructTy)->getElementType(0));
        if (!outerArrayTy) return nullptr;
    
        auto* elementTy = outerArrayTy->getElementType();
        auto* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
        auto* outerIdx = llvm::ConstantInt::get(builder.getInt32Ty(), outerIndex);
    
        auto* outerElemPtr = builder.CreateGEP(outerArrayTy, outerArrayPtr, {zero, outerIdx}, "outer_elem_ptr");
        if (auto* innerStructTy = llvm::dyn_cast<llvm::StructType>(elementTy)) {
            auto* innerArrayTy = llvm::dyn_cast<llvm::ArrayType>(innerStructTy->getElementType(0));
            if (!innerArrayTy) return nullptr;
    
            auto* innerArrayPtr = builder.CreateStructGEP(innerStructTy, outerElemPtr, 0, "inner_array_ptr");
            auto* innerIdxVal = llvm::ConstantInt::get(builder.getInt32Ty(), innerIndex);
            return builder.CreateGEP(innerArrayTy, innerArrayPtr, {zero, innerIdxVal}, "nested_elem_ptr");
        }
    
        return outerElemPtr;
    }
    

    bool ArrayType::equals(const BaseType* other) const {
        if (other->getTypeTag() != DataType::ARRAY) return false;
        const auto* otherArray = dynamic_cast<const ArrayType*>(other);
        size_t numEle =  getNumElements();
        auto* eleType = getElementType();
        return otherArray && numEle == otherArray->getNumElements() && eleType->equals(otherArray->getElementType);
    }

    const BaseType* ArrayType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* ArrayType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* ArrayType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t ArrayType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t ArrayType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags ArrayType::getDIFlags() const { return llvm::DINode::FlagZero; }

}
