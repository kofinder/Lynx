/**
 * @file VoidType.hpp
 * @brief Defines the VoidType class representing the built-in void type in the Lynx type system.
 *
 * The `VoidType` represents the absence of a value in the Lynx language.
 * It is used primarily as the return type for functions that do not return a value.
 *
 * **Key Responsibilities:**
 * - Represents "no value" or "void" in function signatures.
 * - Generates the corresponding LLVM IR type (`llvm::Type::getVoidTy()`).
 * - Integrates with `TypeVisitor` for semantic analysis.
 * - Produces DWARF-compatible debug information for void types.
 *
 * **Integration Points:**
 * - Used in function declarations, method signatures, and control flow where no return value is expected.
 * - Cannot be instantiated or assigned.
 * - Default value is effectively nonexistent.
 *
 * **LLVM Details:**
 * - Maps to LLVM `void` type.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - No default LLVM value.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_VOID_TYPE_HPP
#define LYNX_VOID_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class VoidType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit VoidType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            VoidType(const VoidType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~VoidType() override = default;
            VoidType& operator=(const VoidType&) = delete;
            VoidType(VoidType&&) = delete;
            VoidType& operator=(VoidType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<VoidType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            TypeMethodResolver* getOrCreateResolver() const override;
            
            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::VOID; }

            std::string getDebugName() const override { return "void"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}
#endif 
