/**
 * @file ShortType.hpp
 * @brief Defines the ShortType class representing the built-in short integer type in the Lynx type system.
 *
 * The `ShortType` encapsulates signed 16-bit integers within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents 16-bit signed integers (`short` in C/C++).
 * - Generates the corresponding LLVM IR type (`llvm::Type::getInt16Ty()`).
 * - Supports variable instantiation, assignment, and literal value creation.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in arithmetic operations, loops, array indexing, and numeric expressions.
 * - Interoperates with `ByteType` and `IntegerType` for implicit conversions.
 * - Default initialization yields zero (`0`).
 *
 * **LLVM Details:**
 * - Maps to 16-bit integer (`i16`) type.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Default LLVM value: `ConstantInt::get(Type::getInt16Ty(), 0)`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SHORT_TYPE_HPP
#define LYNX_SHORT_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class ShortType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;
            
        public:
        
            // Use explicit constructor for RAII
            explicit ShortType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            ShortType(const ShortType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~ShortType() override = default;
            ShortType& operator=(const ShortType&) = delete;
            ShortType(ShortType&&) = delete;
            ShortType& operator=(ShortType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<ShortType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            TypeMethodResolver* getOrCreateResolver() const  override;
            
            void accept(TypeVisitor& visitor) override;

            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return shortMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::SHORT; }

            std::string getDebugName() const override { return "string"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif 
