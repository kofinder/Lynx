/**
 * @file StringType.hpp
 * @brief Defines the StringType class representing the built-in string type in the Lynx type system.
 *
 * The `StringType` encapsulates text data within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents sequences of characters (`String` in Lynx).
 * - Generates the corresponding LLVM IR type (typically a pointer to character array or custom string struct).
 * - Supports variable instantiation, assignment, and literal value creation.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in string concatenation, printing, comparisons, and indexing.
 * - Interoperates with `CharType` for single-character operations.
 * - Default initialization yields an empty string (`""`).
 *
 * **LLVM Details:**
 * - Maps to LLVM representation for string storage (e.g., `i8*` or custom struct pointer).
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Default LLVM value: typically a null pointer or empty string constant.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_STRING_TYPE_HPP
#define LYNX_STRING_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class StringType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit StringType(AstContext* context) : BuiltInType(context) {}

            void accept(TypeVisitor& visitor) override;

            inline DataType getTypeTag() const override { return DataType::STRING; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<StringType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~StringType() override {}
    };
}

#endif 
