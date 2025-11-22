/**
 * @file PointerType.hpp
 * @brief Defines the `PointerType` class representing pointer types in the Lynx type system.
 *
 * The `PointerType` class models pointer semantics, including LLVM type generation,
 * instance creation, assignment, and value handling. It extends `WrapperType` and
 * provides specialized methods for debug metadata, type comparisons, and cloning.
 *
 * **Key Responsibilities:**
 * - Represents pointer types (`T*`) in Lynx.
 * - Generates LLVM IR types and default values.
 * - Supports instance creation, assignment, and value operations.
 * - Emits DWARF-compliant debug information for pointers.
 * - Supports cloning and type comparison operations.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_POINTER_TYPE_HPP
#define LYNX_POINTER_TYPE_HPP

#include <types/interfaces/WrapperType.hpp>

namespace LynxTypes {

    class PointerType : public WrapperType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit PointerType(AstContext* context) : WrapperType(context) {};
            ~PointerType() override = default;

            DataType getTypeTag() const override { return DataType::POINTER; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(LValueType value) const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<PointerType>(*this); }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif
