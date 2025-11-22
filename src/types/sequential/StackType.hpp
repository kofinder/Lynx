/**
 * @file StackType.hpp
 * @brief Defines the StackType class representing dynamically sized LIFO stacks in the Lynx type system.
 *
 * The `StackType` encapsulates sequences of elements with last-in-first-out (LIFO) semantics,
 * supporting push, pop, indexing, and LLVM IR generation for stack operations.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized stacks with elements of a specific type.
 * - Supports variable instantiation, assignment, value creation, and dynamic element management.
 * - Provides utilities for element access and manipulation in LIFO order.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for stack types.
 *
 * **Integration Points:**
 * - Used in stack declarations, push/pop operations, and iteration.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for stack elements.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for dynamic storage and element access.
 * - Provides pointer types and efficient indexing mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying the element type.
 * - Supports type equality checks and cloning for stack types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_STACK_TYPE_HPP
#define LYNX_STACK_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class StackType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit StackType(AstContext* context) : SequentialType(context) {}

            DataType getTypeTag() const override { return DataType::STACK; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;
            
            const BaseType* getElementType() const override { return elementType; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<StackType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "stack"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~StackType() override = default;
    };
}

#endif
