#include <types/sequential/ArrayType.hpp>
#include <context/AstContext.hpp>

using namespace LynxContext;
using namespace LynxTypes;

namespace LynxTypes {

    llvm::Type* ArrayType::computeLLVMType() const {

        if(!elementType) {
            LOG_ERROR("Element type is null.");
            return nullptr;
        }
    
        llvm::Type* elemLLVMType = elementType->getLLVMType();
        if (!elemLLVMType) {
            LOG_ERROR("Failed to get LLVM type for elementType");
            return nullptr;
        }

        if (numElements <= 0) {
            LOG_ERROR("Array size must be greater than 0. Got {}.", numElements);
            return nullptr;
        }    
        
        auto& context = astContext->getLLVMContext();
    
        // If the element type is already a struct wrapping an array, treat it as inner array struct
        if (llvm::isa<llvm::StructType>(elemLLVMType)) {
            // inner_array = struct wrapping elemLLVMType
            llvm::StructType* innerStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::innerArray);
            if (!innerStruct) {
                innerStruct = llvm::StructType::create(context, elemLLVMType, MetadataTypeConstants::innerArray);
            }
    
            // outer_array = struct wrapping [numElements x innerStruct]
            llvm::ArrayType* outerArrayTy = llvm::ArrayType::get(innerStruct, numElements);
    
            llvm::StructType* outerStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::outerArray);
            if (!outerStruct) {
                outerStruct = llvm::StructType::create(context, outerArrayTy, MetadataTypeConstants::outerArray);
            }
    
            cachedLLVMType = outerStruct;
            return cachedLLVMType;
        }
    
        // Base case: just wrap array in a struct (like your %lynx.array.type = type { [3 x i32] })
        llvm::ArrayType* arrayType = llvm::ArrayType::get(elemLLVMType, numElements);
    
        llvm::StructType* arrayStruct = llvm::StructType::getTypeByName(context, MetadataTypeConstants::simpleArray);
        if (!arrayStruct) {
            arrayStruct = llvm::StructType::create(context, arrayType, MetadataTypeConstants::simpleArray);
        }
    
        cachedLLVMType = arrayStruct;
        return cachedLLVMType;
    }

    llvm::Value* ArrayType::createValue(std::vector<llvm::Value*> values) const {
        LOG_INFO("Invoked... arrsize {} ", numElements);

        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
        llvm::Type* computedType = computeLLVMType();
    
        if (!computedType) {
            LOG_ERROR("computeLLVMType returned null");
            return nullptr;
        }
    
        if (values.empty()) {
            return llvm::UndefValue::get(computedType);
        }
    
        bool allConstants = std::all_of(values.begin(), values.end(),
            [](llvm::Value* v) { return llvm::isa<llvm::Constant>(v); });
    
        // Handle outer struct wrapping array of inner struct
        if (auto* outerStruct = llvm::dyn_cast<llvm::StructType>(computedType)) {
            llvm::Type* innerType = outerStruct->getElementType(0);
            if (auto* arrayTy = llvm::dyn_cast<llvm::ArrayType>(innerType)) {
                llvm::Type* innerStructTy = arrayTy->getElementType();
    
                // If the innerStructTy is a struct (like your inner_array), values must be constants for nested
                if (llvm::isa<llvm::StructType>(innerStructTy)) {
                    if (!allConstants) {
                        LOG_ERROR("Nested array initialization requires constant values");
                        return nullptr;
                    }
                    // values are constants of innerStruct type, build constant array and wrap in constant struct
                    llvm::SmallVector<llvm::Constant*, 8> constElems;
                    for (auto* val : values) {
                        constElems.push_back(llvm::cast<llvm::Constant>(val));
                    }
                    llvm::Constant* constArray = llvm::ConstantArray::get(arrayTy, constElems);
                    return llvm::ConstantStruct::get(outerStruct, constArray);
                }
                // Otherwise fallback (should not happen if types consistent)
            }
    
            // Base case: single struct wrapping array of primitives
            if (allConstants) {
                llvm::SmallVector<llvm::Constant*, 8> constElems;
                for (auto* val : values) {
                    constElems.push_back(llvm::cast<llvm::Constant>(val));
                }
                llvm::Constant* constArray = llvm::ConstantArray::get(
                    llvm::cast<llvm::ArrayType>(outerStruct->getElementType(0)), constElems);
                return llvm::ConstantStruct::get(outerStruct, constArray);
            } else {
                // For non-constant values, build insert operations (less common for nested structs)
                llvm::Value* undefStruct = llvm::UndefValue::get(outerStruct);
                llvm::Value* undefArray = builder.CreateExtractValue(undefStruct, {0});
                for (unsigned i = 0; i < values.size(); ++i) {
                    undefArray = builder.CreateInsertValue(undefArray, values[i], {i});
                }
                return builder.CreateInsertValue(undefStruct, undefArray, {0});
            }
        }
    
        LOG_ERROR("Unhandled type in createValue");
        return llvm::UndefValue::get(computeLLVMType());
    }

    llvm::Value* ArrayType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* llvmType = computeLLVMType();
        auto var = builder.CreateAlloca(llvmType, nullptr, variableName); 
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::arrayType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }       
        return var;
    }

    llvm::Type* ArrayType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return computeLLVMType()->getPointerTo();
    }

    llvm::Value* ArrayType::getDefaultValue() {
        LOG_INFO("Invoked...");
        llvm::Type* llvmArrayType = computeLLVMType();
        return llvm::ConstantAggregateZero::get(llvmArrayType);
    }

    llvm::Value* ArrayType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* ArrayType::getElementPointer(llvm::Value* arrayAlloca, int index) const {
        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
    
        llvm::Type* structType = computeLLVMType();
        if (!llvm::isa<llvm::StructType>(structType)) {
            LOG_ERROR("ArrayType is not a struct");
            return nullptr;
        }
    
        // GEP to struct field 0 (which holds the array)
        llvm::Value* arrayPtr = builder.CreateStructGEP(structType, arrayAlloca, 0, "array_gep");
    
        llvm::Type* innerType = llvm::cast<llvm::StructType>(structType)->getElementType(0);
    
        // Handle nested struct case
        if (auto* outerArrayTy = llvm::dyn_cast<llvm::ArrayType>(innerType)) {
            llvm::Type* elementTy = outerArrayTy->getElementType();
    
            // Nested array: struct { [N x struct { [M x i32] }] }
            if (auto* innerStruct = llvm::dyn_cast<llvm::StructType>(elementTy)) {
                llvm::Value* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
                llvm::Value* idx = llvm::ConstantInt::get(builder.getInt32Ty(), index);
    
                // GEP to [N x struct] -> struct at index
                llvm::Value* structElemPtr = builder.CreateGEP(outerArrayTy, arrayPtr, {zero, idx}, "outer_elem_ptr");
    
                // GEP to inner struct field 0 -> [M x i32]
                llvm::Value* innerArrayPtr = builder.CreateStructGEP(innerStruct, structElemPtr, 0, "inner_array_ptr");
    
                return innerArrayPtr;
            }
        }
    
        // Base case: single-level struct { [N x i32] }
        llvm::Value* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
        llvm::Value* idx = llvm::ConstantInt::get(builder.getInt32Ty(), index);
        return builder.CreateGEP(innerType, arrayPtr, {zero, idx}, "elem_ptr");
    }

    llvm::Value* ArrayType::getElementPointer(llvm::Value* arrayAlloca, int outerIndex, int innerIndex) const {
        auto& builder = astContext->getBuilder();
        auto& context = astContext->getLLVMContext();
    
        llvm::Type* outerStructTy = computeLLVMType();
    
        if (!llvm::isa<llvm::StructType>(outerStructTy)) {
            LOG_ERROR("Expected a struct type for array.");
            return nullptr;
        }
    
        // Get pointer to the inner array: struct { [N x T] } => GEP to index 0
        llvm::Value* outerArrayPtr = builder.CreateStructGEP(outerStructTy, arrayAlloca, 0, "outer_struct.array");
    
        llvm::ArrayType* outerArrayTy = llvm::dyn_cast<llvm::ArrayType>(
            llvm::cast<llvm::StructType>(outerStructTy)->getElementType(0));
        if (!outerArrayTy) {
            LOG_ERROR("Outer field is not an array");
            return nullptr;
        }
    
        llvm::Type* elementTy = outerArrayTy->getElementType();
    
        llvm::Value* zero = llvm::ConstantInt::get(builder.getInt32Ty(), 0);
        llvm::Value* outerIdx = llvm::ConstantInt::get(builder.getInt32Ty(), outerIndex);
    
        // GEP to outer array element (could be struct or scalar)
        llvm::Value* outerElemPtr = builder.CreateGEP(outerArrayTy, outerArrayPtr, {zero, outerIdx}, "outer_elem_ptr");
    
        // If nested: element is a struct wrapping [M x T]
        if (auto* innerStructTy = llvm::dyn_cast<llvm::StructType>(elementTy)) {
            llvm::ArrayType* innerArrayTy = llvm::dyn_cast<llvm::ArrayType>(innerStructTy->getElementType(0));
            if (!innerArrayTy) {
                LOG_ERROR("Inner struct does not contain an array.");
                return nullptr;
            }
    
            // GEP to inner array inside struct field
            llvm::Value* innerArrayPtr = builder.CreateStructGEP(innerStructTy, outerElemPtr, 0, "inner_array_ptr");
    
            llvm::Value* innerIdxVal = llvm::ConstantInt::get(builder.getInt32Ty(), innerIndex);
            return builder.CreateGEP(innerArrayTy, innerArrayPtr, {zero, innerIdxVal}, "nested_elem_ptr");
        }
    
        // If not nested, just return pointer to scalar element
        return outerElemPtr;
    }
    

    bool ArrayType::equals(const BaseType* other) const {
        if (other->getTypeTag() != DataType::ARRAY) {
            return false;
        }
        auto* otherArray = dynamic_cast<const ArrayType*>(other);
        return otherArray &&
        numElements == otherArray->numElements &&
            elementType->equals(otherArray->elementType);
    }

    const BaseType* ArrayType::createWithStatic(bool newIsStatic) const {
        auto clone = std::make_shared<ArrayType>(astContext);
        clone->setElementType(elementType);
        clone->createWithStatic(newIsStatic);
        clone->createWithConst(isConst());
        return clone.get();
    }

    const BaseType* ArrayType::createWithConst(bool newIsConst) const {
        auto clone = std::make_shared<ArrayType>(astContext);
        clone->setElementType(elementType);
        clone->setConst(newIsConst);
        clone->setStatic(isStatic());
        return clone.get();
    }

    std::string ArrayType::getDebugName() const {
        return "array[" + std::to_string(numElements) + "] of " + elementType->getDebugName();
    }

    llvm::DIType* ArrayType::getDIType(llvm::DIScope* scope) const {
        auto& diBuilder = astContext->getDebugBuilder();
        auto* elementDIType = elementType->getDIType(scope);
        
        uint64_t elemSizeBits = elementType->getDebugSizeInBits();
        uint32_t alignBits = elementType->getDebugAlignInBits();
        uint64_t arraySizeBits = elemSizeBits * numElements;
    
        llvm::DINodeArray subscriptArray = diBuilder.getOrCreateArray({
            diBuilder.getOrCreateSubrange(0, numElements)
        });

        return diBuilder.createArrayType(
            arraySizeBits,
            alignBits,
            elementDIType,
            subscriptArray
        );
    }

    uint64_t ArrayType::getDebugSizeInBits() const  {
        return elementType->getDebugSizeInBits() * numElements;
    }

    uint32_t ArrayType::getDebugAlignInBits() const {
        return elementType->getDebugAlignInBits();
    }

    llvm::DINode::DIFlags ArrayType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}