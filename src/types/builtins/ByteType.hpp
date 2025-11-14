/**
 * @file ByteType.hpp
 * @brief Defines the ByteType class representing the built-in 8-bit byte type in the Lynx type system.
 *
 * The `ByteType` class models the `byte` primitive type within the Lynx language.
 * It extends `BuiltInType` to provide LLVM IR generation, default value construction,
 * assignment handling, and DWARF-compatible debug metadata.
 *
 * **Key Responsibilities:**
 * - Represents 8-bit unsigned integer values (`0–255`).
 * - Provides LLVM IR type generation (`i8`).
 * - Implements default initialization (`0`) and assignment operations.
 * - Supports cloning and visitor integration for semantic analysis.
 * - Emits debug type information for tooling and debuggers.
 *
 * **Integration Points:**
 * - Used in code generation for low-level operations, buffers, and memory manipulation.
 * - Commonly used as the base element type for byte arrays and streams.
 * - Works with `TypeVisitor` and `TypeMethodResolver` to expose language-level methods or operators.
 *
 * **LLVM Details:**
 * - Maps to `llvm::Type::getInt8Ty()` for representation.
 * - Default value: zero-initialized `i8`.
 * - Size: 8 bits; alignment determined by target data layout.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BYTE_TYPE_HPP
#define LYNX_BYTE_TYPE_HPP

#include  <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {
    
    class ByteType: public BuiltInType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit ByteType(AstContext* context) : BuiltInType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string, int>& getMethodRegistry() const override;

            llvm::Value* emitMethodCall(llvm::Value* instance, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ByteType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::BYTE; }

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~ByteType() override = default;
    };
}

#endif 
