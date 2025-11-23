/**
 * @file ListType.hpp
 * @brief Defines the ListType class representing dynamically sized lists in the Lynx type system.
 *
 * The `ListType` encapsulates sequences of elements of a single type with dynamic sizing,
 * supporting indexing, element access, and LLVM IR generation for list operations.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized lists with elements of a specific type.
 * - Supports variable instantiation, value creation, assignment, and dynamic resizing.
 * - Provides utilities to access elements via indexing.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for list types.
 *
 * **Integration Points:**
 * - Used in list declarations, indexing, iteration, and dynamic collections.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for list elements.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for dynamic storage and element pointers.
 * - Provides pointer types and efficient indexing mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying the element type.
 * - Supports type equality checks and cloning for list types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_LIST_TYPE_HPP
#define LYNX_LIST_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class ListType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit ListType(AstContext* context) : SequentialType(context) {}

            // Public copy constructor, needed for clone()
            ListType(const ListType& other) : SequentialType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~ListType() override = default;
            ListType& operator=(const ListType&) = delete;
            ListType(ListType&&) = delete;
            ListType& operator=(ListType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<ListType>(*this);
            }

            bool isIndexable() const noexcept override { return true; }

            DataType getTypeTag() const override { return DataType::LIST; }

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "list"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif
