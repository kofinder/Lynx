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
 * @date: November 2, 2024
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

            explicit VoidType(AstContext* context) : BuiltInType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            // void accept(TypeVisitor& visitor) override;

            // const std::unordered_map<std::string_view, int>& getMethodRegistry() const override;

            // llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<VoidType>(*this); }

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::VOID; }

            std::string getDebugName() const override { return "void"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~VoidType() override = default;
    };
}
#endif 
