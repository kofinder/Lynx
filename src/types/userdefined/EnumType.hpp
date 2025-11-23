/**
 * @file EnumType.hpp
 * @brief Defines the EnumType class representing user-defined enumerations in the Lynx type system.
 *
 * The `EnumType` encapsulates named constant values (enumerators) within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents user-defined enumerations (`enum`) in Lynx.
 * - Tracks enum members and their associated values.
 * - Generates LLVM IR types for enum instances, including optional union types for storage.
 * - Supports variable instantiation, assignment, and default value creation.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for enums.
 *
 * **Integration Points:**
 * - Used in variable declarations, assignments, comparisons, and switch/case expressions.
 * - Supports member lookup by name and optional global constant registration for each member.
 * - Interoperates with other integer or numeric types when necessary.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` and optionally a union type for enum value storage.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Maintains a mapping from LLVM type to `EnumType` for retrieval.
 *
 * **Additional Features:**
 * - Provides utilities to add, retrieve, and enumerate enum members.
 * - Supports optional registration of global LLVM constants for enum members.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ENUM_TYPE_HPP
#define LYNX_ENUM_TYPE_HPP

#include <optional>
#include "helper/EnumMember.hpp"
#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class EnumType : public UserDefinedType {

        private:

            std::string enumName;

            mutable std::string cachedFullName;

            mutable llvm::StructType* cachedType = nullptr;

            mutable llvm::StructType* cachedUnionType = nullptr;

            mutable llvm::PointerType* cachedPointerType = nullptr;

            std::unordered_map<std::string, EnumMember> members;

            static inline std::unordered_map<const llvm::StructType*, const EnumType*> llvmTypeToClass;
    
        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit EnumType(
                AstContext* context, 
                std::string name
            ) : UserDefinedType(context), enumName(std::move(name)) {}

            // Public copy constructor, needed for clone()
            EnumType(const EnumType& other) : UserDefinedType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~EnumType() override = default;
            EnumType& operator=(const EnumType&) = delete;
            EnumType(EnumType&&) = delete;
            EnumType& operator=(EnumType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::StructType* createEnumValueUnion(llvm::LLVMContext& context) const;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::ENUM; }

            std::string getDebugName() const override { return "enum"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            const std::string& qualifiedName() const;
            const std::string& originalName() const { return enumName; }

            void registerLLVMType(llvm::StructType* structTy) const;
            static const EnumType* fromLLVMType(const llvm::Type* type);

            std::optional<EnumMember> getMember(const std::string& name) const;
            
            void addMember(const std::string& name, const EnumMember& member);

            const std::unordered_map<std::string, EnumMember>& getAllMembers() const { return members; }

            void registerGlobalConstant(const std::string& memberName, llvm::GlobalVariable* gov) const;
    };
}

#endif 
