/**
 * @file VectorType.hpp
 * @brief Defines the VectorType class representing dynamically sized, indexable vectors in the Lynx type system.
 *
 * The `VectorType` encapsulates sequences of elements that are dynamically sized and support
 * random access via indices. Vectors differ from arrays by being resizable while maintaining
 * contiguous storage semantics.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized vectors with elements of a specific type.
 * - Supports variable instantiation, assignment, and value creation.
 * - Provides utilities for element access by index.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for vector types.
 *
 * **Integration Points:**
 * - Used in vector declarations, indexing, iteration, and dynamic resizing operations.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for vector elements.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for dynamic storage and element access.
 * - Provides pointer types and efficient indexing mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying the element type.
 * - Supports type equality checks and cloning for vector types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_VECTOR_TYPE_HPP
#define LYNX_VECTOR_TYPE_HPP

#include <unordered_set>
#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class VectorType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

            std::string getSafeStructName() const;

            std::string getSafeStructName(std::unordered_set<const BaseType*>& visited) const;

        public:

            explicit VectorType(AstContext* context) : SequentialType(context) {}

            inline bool isIndexable() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::VECTOR; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            const BaseType* getElementType() const override { return elementType; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<VectorType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "vector"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~VectorType() override = default;
    };
}

#endif
