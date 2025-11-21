/**
 * @file ArrayType.hpp
 * @brief Defines the ArrayType class representing fixed-size or dynamically sized arrays in the Lynx type system.
 *
 * The `ArrayType` encapsulates sequences of elements of a single type, supporting
 * indexing, element access, and LLVM IR generation for array operations.
 *
 * **Key Responsibilities:**
 * - Represents arrays of a specific element type and size.
 * - Supports variable instantiation, value creation, and assignment for arrays.
 * - Provides utilities to compute element pointers for single or multi-dimensional arrays.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for array types.
 *
 * **Integration Points:**
 * - Used in array declarations, indexing operations, loops, and multi-dimensional arrays.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for array elements.
 *
 * **LLVM Details:**
 * - Maps to `llvm::ArrayType` or sequential LLVM types depending on element type and size.
 * - Provides pointer types and element pointers for efficient LLVM IR code generation.
 * - Supports both single and multi-dimensional indexing.
 *
 * **Additional Features:**
 * - Provides methods for querying element type and number of elements.
 * - Supports type equality checks and cloning for array types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ARRAY_TYPE_HPP
#define LYNX_ARRAY_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {
    

    class ArrayType : public SequentialType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;
            
        public:

            explicit ArrayType(AstContext* context) : SequentialType(context) {}
            
            inline bool isIndexable() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::ARRAY; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            llvm::Value* getElementPointer(llvm::Value* arrayAlloca, int index) const;
            
            llvm::Value* getElementPointer(llvm::Value* arrayAlloca, int outerIndex, int innerIndex) const;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ArrayType>(*this); }

            const BaseType* getElementType() const override { return elementType; }

            const size_t getNumElements() const { return numElements; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "vector"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    
            ~ArrayType() override = default;
    };
}

#endif 
