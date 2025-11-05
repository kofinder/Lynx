/**
 * @file DictionaryType.hpp
 * @brief Defines the DictionaryType class representing associative key-value maps in the Lynx type system.
 *
 * The `DictionaryType` encapsulates collections that map unique keys to corresponding values.
 * It supports insertion, lookup, deletion, and LLVM IR generation for associative operations.
 *
 * **Key Responsibilities:**
 * - Represents dynamically sized key-value maps with specific key and value types.
 * - Supports variable instantiation, assignment, and value creation.
 * - Provides utilities for key-based element access and manipulation.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for dictionary types.
 *
 * **Integration Points:**
 * - Used in dictionary declarations, key-based access, and iteration.
 * - Keys and values can be of any type, including user-defined and built-in types.
 * - Provides default values for keys and values where appropriate.
 *
 * **LLVM Details:**
 * - Maps to appropriate LLVM types for associative storage and key-value access.
 * - Provides pointer types and efficient mechanisms for LLVM IR code generation.
 *
 * **Additional Features:**
 * - Supports querying both key and value types.
 * - Supports type equality checks and cloning for dictionary types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_DICTIONARY_TYPE_HPP
#define LYNX_DICTIONARY_TYPE_HPP

#include <types/interfaces/AssociativeType.hpp>

namespace LynxTypes {

    class DictionaryType : public AssociativeType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit DictionaryType(AstContext* context) : AssociativeType(context) {}

            inline bool isAssociative() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::MAP; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> paris) const override;

            void setElementType(BaseType* eleType) override { elementType = eleType; }

            const BaseType* getElementType() const override { return elementType; }

            void setValueType(BaseType* value) override { elementValue = value; }

            const BaseType* getValueType() const override { return elementValue; }

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DictionaryType>(*this);}
        
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~DictionaryType() override = default;
    };
}

#endif
