/**
 * @file DateType.hpp
 * @brief Defines the DateType class representing the built-in date type in the Lynx type system.
 *
 * The `DateType` encapsulates calendar date values within the Lynx language.
 * It provides mechanisms for LLVM IR generation, field access, semantic analysis,
 * and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents date objects, typically including year, month, and day fields.
 * - Supports variable instantiation, field access, assignment, and default value creation.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for date types.
 *
 * **Integration Points:**
 * - Used in date manipulation, comparisons, and date-based calculations.
 * - Supports access to individual date components (e.g., year, month, day).
 * - Default initialization yields a zeroed or epoch-equivalent date.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` for representing the date object.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Maintains a static cached LLVM type for performance.
 *
 * **Additional Features:**
 * - Provides utilities for field access, type comparison, cloning, and debug name resolution.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_DATE_TYPE_HPP
#define LYNX_DATE_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class DateType : public UserDefinedType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit DateType(AstContext* context) : UserDefinedType(context) {}

            inline DataType getTypeTag() const override { return DataType::DATE; }

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* getField(std::string fieldName, llvm::Value* instance);

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override;

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DateType>(*this); }

            ~DateType() override = default;
    };
}
#endif 
