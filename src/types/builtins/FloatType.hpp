/**
 * @file FloatType.hpp
 * @brief Defines the FloatType class representing the built-in floating-point type in the Lynx type system.
 *
 * The `FloatType` encapsulates single-precision floating-point numbers within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents 32-bit IEEE 754 floating-point values (`float` in C/C++).
 * - Generates corresponding LLVM IR type (`llvm::Type::getFloatTy()`).
 * - Supports variable instantiation, assignment, and value creation from literals.
 * - Interacts with `TypeVisitor` and `TypeMethodResolver` for semantic and runtime method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in arithmetic operations, numeric expressions, and mathematical standard libraries.
 * - Often interacts with `DoubleType` for implicit conversions and mixed-type arithmetic.
 * - Supports default initialization to zero (`0.0f`).
 *
 * **LLVM Details:**
 * - Maps to 32-bit floating-point (`float`) type.
 * - Provides pointer type and size/alignment from target `DataLayout`.
 * - Default LLVM value: `ConstantFP::get(Type::getFloatTy(), 0.0)`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FLOAT_TYPE_HPP
#define LYNX_FLOAT_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class FloatType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit FloatType(AstContext* context) : BuiltInType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;
            
            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return floatMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<FloatType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::FLOAT; }

            std::string getDebugName() const override { return "float"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~FloatType() override = default;
    };
}

#endif 
