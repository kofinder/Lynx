/**
 * @file LongType.hpp
 * @brief Defines the LongType class representing the built-in long integer type in the Lynx type system.
 *
 * The `LongType` encapsulates signed 64-bit integers within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents 64-bit signed integers (`long` in C/C++).
 * - Generates the corresponding LLVM IR type (`llvm::Type::getInt64Ty()`).
 * - Supports variable instantiation, assignment, and literal value creation.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in arithmetic operations, loops, array indexing, and numeric expressions.
 * - Interoperates with `IntegerType`, `ShortType`, and `ByteType` for implicit conversions.
 * - Default initialization yields zero (`0L`).
 *
 * **LLVM Details:**
 * - Maps to 64-bit integer (`i64`) type.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Default LLVM value: `ConstantInt::get(Type::getInt64Ty(), 0)`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_LONG_TYPE_HPP
#define LYNX_LONG_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class LongType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;
            
        public:

            explicit LongType(AstContext* context) : BuiltInType(context) {}

            void accept(TypeVisitor& visitor) override;

            inline DataType getTypeTag() const override { return DataType::LONG; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            std::unique_ptr<TypeMethodResolver> createMethodResolver() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<LongType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~LongType() override = default;
    };
}

#endif 
