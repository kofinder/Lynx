/**
 * @file ReferenceType.hpp
 * @brief Defines the `ReferenceType` class representing reference types in the Lynx type system.
 *
 * The `ReferenceType` class models reference semantics (`T&`) in Lynx. It extends
 * `WrapperType` and provides methods for LLVM type generation, instance creation,
 * assignment, value handling, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents reference types in Lynx programs.
 * - Generates LLVM IR types and default reference values.
 * - Supports instance creation, assignment, and value operations.
 * - Emits DWARF-compliant debug information for references.
 * - Supports cloning and type comparison operations.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */


#ifndef LYNX_REFRENCE_TYPE_HPP
#define LYNX_REFRENCE_TYPE_HPP

#include <types/interfaces/WrapperType.hpp>

namespace LynxTypes {

    class ReferenceType : public WrapperType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit ReferenceType(AstContext* context) : WrapperType(context) {};

            DataType getTypeTag() const override { return DataType::REFERENCE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<ReferenceType>(*this); }

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~ReferenceType() override = default;
    };
}

#endif
