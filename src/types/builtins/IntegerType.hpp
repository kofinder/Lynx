/**
 * @file IntegerType.hpp
 * @brief Defines the IntegerType class representing the built-in integer type in the Lynx type system.
 *
 * The `IntegerType` encapsulates signed 32-bit integers within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents 32-bit signed integers (`int` in C/C++).
 * - Generates the corresponding LLVM IR type (`llvm::Type::getInt32Ty()`).
 * - Supports variable instantiation, assignment, and literal value creation.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in arithmetic operations, loops, array indexing, and numeric expressions.
 * - Often interoperates with `ByteType`, `ShortType`, and `LongType` for implicit conversions.
 * - Default initialization yields zero (`0`).
 *
 * **LLVM Details:**
 * - Maps to 32-bit integer (`i32`) type.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Default LLVM value: `ConstantInt::get(Type::getInt32Ty(), 0)`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_INTEGER_TYPE_HPP
#define LYNX_INTEGER_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class IntegerType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit IntegerType(AstContext* context) : BuiltInType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string, int>& getMethodRegistry() const override;

            llvm::Value* emitMethodCall(llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<IntegerType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::INT; }

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~IntegerType() override {}
    };
}
#endif 
