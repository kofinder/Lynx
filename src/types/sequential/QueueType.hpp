/**
 * @file QueueType.hpp
 * @brief Defines the QueueType class representing dynamically sized FIFO queues in the Lynx type system.
 *
 * The `QueueType` encapsulates sequences of elements with first-in-first-out (FIFO) semantics,
 * supporting enqueue, dequeue, indexing, and LLVM IR generation for queue operations.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized queues with elements of a specific type.
 * - Supports variable instantiation, value creation, assignment, and dynamic element management.
 * - Provides utilities for element access and manipulation in FIFO order.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for queue types.
 *
 * **Integration Points:**
 * - Used in queue declarations, enqueue/dequeue operations, and iteration.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for queue elements.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for dynamic storage and element access.
 * - Provides pointer types and efficient indexing mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying the element type.
 * - Supports type equality checks and cloning for queue types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_QUEUE_TYPE_HPP
#define LYNX_QUEUE_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class QueueType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit QueueType(AstContext* context) : SequentialType(context) {}

            inline DataType getTypeTag() const override { return DataType::QUEUE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;
            
            const BaseType* getElementType() const override { return elementType; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<QueueType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~QueueType() override = default;
    };
}

#endif
