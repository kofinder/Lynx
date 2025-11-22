/**
 * @file DateTimeType.hpp
 * @brief Defines the DateTimeType class representing the built-in date-time type in the Lynx type system.
 *
 * The `DateTimeType` encapsulates calendar date and time values within the Lynx language.
 * It provides mechanisms for LLVM IR generation, field access, semantic analysis,
 * and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents date-time objects, typically including year, month, day, hour, minute, and second fields.
 * - Supports variable instantiation, field access, assignment, and default value creation.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for date-time types.
 *
 * **Integration Points:**
 * - Used in date-time manipulation, comparisons, arithmetic, and scheduling operations.
 * - Supports access to individual date and time components.
 * - Default initialization yields a zeroed or epoch-equivalent date-time value.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` for representing the date-time object.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Maintains a static cached LLVM type for performance.
 *
 * **Additional Features:**
 * - Provides utilities for field access, type comparison, cloning, and debug name resolution.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_DATE_TIME_TYPE_HPP
#define LYNX_DATE_TIME_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class DateTimeType : public UserDefinedType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:
        
            explicit DateTimeType(AstContext* context) : UserDefinedType(context) {}
            ~DateTimeType() override = default;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            std::unique_ptr<BaseType> clone() const override { return std::make_unique<DateTimeType>(*this); }

            bool equals(const BaseType* other) const override;

            llvm::Value* getField(const std::string& fieldName, llvm::Value* instance);

            DataType getTypeTag() const override { return DataType::DATETIME; }

            std::string getDebugName() const override { return "Datetime"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}
#endif 
