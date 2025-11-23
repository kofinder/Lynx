/**
 * @file BooleanType.hpp
 * @brief Defines the BooleanType class representing the built-in boolean type in the Lynx type system.
 *
 * The `BooleanType` class models the `bool` type in the Lynx programming language.
 * It extends the `BuiltInType` base class, providing LLVM IR type generation, 
 * codegen behavior for assignments and instantiation, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents boolean literals and variables (`true`, `false`).
 * - Provides LLVM IR type mapping (i1).
 * - Implements default initialization and assignment operations.
 * - Supports conversion to string (for output operations such as `println`).
 * - Emits DWARF-compliant debug type information for booleans.
 *
 * **Integration Points:**
 * - Used by the semantic analyzer and IR builder for type checking and codegen.
 * - Resolved via `BuiltInType` factory methods or type inference in expressions.
 * - Compatible with `TypeMethodResolver` for potential boolean-specific methods (e.g., logical operations).
 *
 * **LLVM Details:**
 * - Maps to `llvm::Type::getInt1Ty()` for boolean representation.
 * - Default value: `false`.
 * - Size in bits: 1 (as per LLVM data layout).
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BOOLEAN_TYPE_HPP
#define LYNX_BOOLEAN_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class BooleanType: public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit BooleanType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            BooleanType(const BooleanType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~BooleanType() override = default;
            BooleanType& operator=(const BooleanType&) = delete;
            BooleanType(BooleanType&&) = delete;
            BooleanType& operator=(BooleanType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<BooleanType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return boolMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            const llvm::Value* convertBooleanToString(llvm::Value* value);

            DataType getTypeTag() const override { return DataType::BOOLEAN; }

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "bool"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif 
