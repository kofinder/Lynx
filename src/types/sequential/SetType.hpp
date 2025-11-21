/**
 * @file SetType.hpp
 * @brief Defines the SetType class representing dynamically sized sets in the Lynx type system.
 *
 * The `SetType` encapsulates collections of unique elements, supporting insertion, removal,
 * membership checks, and LLVM IR generation for set operations.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized sets containing unique elements of a specific type.
 * - Supports variable instantiation, assignment, and value creation.
 * - Provides utilities for element access, membership testing, and iteration.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for set types.
 *
 * **Integration Points:**
 * - Used in set declarations, membership operations, iteration, and dynamic collection management.
 * - Works with any type as the element type, including user-defined and built-in types.
 * - Provides default values for set elements.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for dynamic storage and element access.
 * - Provides pointer types and efficient mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying the element type.
 * - Supports type equality checks and cloning for set types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SET_TYPE_HPP
#define LYNX_SET_TYPE_HPP

#include <types/interfaces/SequentialType.hpp>

namespace LynxTypes {

    class SetType : public SequentialType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit SetType(AstContext* context) : SequentialType(context) {}

            inline DataType getTypeTag() const override { return DataType::SET; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;
            
            const BaseType* getElementType() const override { return elementType; }

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<SetType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "set"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~SetType() override = default;
    };
}

#endif
