/**
 * @file AutoType.hpp
 * @brief Defines the AutoType class representing the type-inference placeholder in the Lynx type system.
 *
 * The `AutoType` represents a variable whose type is automatically inferred by the compiler.
 * It serves as a placeholder during semantic analysis and is later replaced with the inferred concrete type.
 *
 * **Key Responsibilities:**
 * - Represents variables declared with `auto` in the Lynx language.
 * - Stores a pointer to the inferred type once determined by type inference.
 * - Supports variable instantiation, assignment, and value creation for various forms (scalar, vector, pairs).
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for inferred types once resolved.
 *
 * **Integration Points:**
 * - Used wherever `auto` declarations appear.
 * - Works seamlessly with built-in and user-defined types once inference is complete.
 * - Provides utilities to query whether a type has been inferred.
 *
 * **LLVM Details:**
 * - Provides LLVM IR type based on the inferred concrete type.
 * - Supports pointer types, default values, and assignment according to the inferred type.
 *
 * **Additional Features:**
 * - Can store scalar, vector, or key-value pair values during code generation.
 * - Supports type equality checks and type acceptance checks once inference is complete.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_AUTO_TYPE_HPP
#define LYNX_AUTO_TYPE_HPP

#include <types/interfaces/BaseType.hpp>

namespace LynxTypes {

    class AutoType : public BaseType {

        private:

            BaseType* inferredType = nullptr;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit AutoType(AstContext* context) : BaseType(context) {};

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* createValue(std::vector<llvm::Value*> values) const override;

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override;

            inline bool isBuiltInType() const noexcept override { return true; }

            inline DataType getTypeTag() const override { return DataType::AUTO; }

            inline void setInferredType(BaseType* type) { inferredType = type; }
            
            inline BaseType* getInferredType() const { return inferredType; }
            
            inline bool isInferred() const { return inferredType != nullptr; }
                        
            bool equals(const BaseType* other) const override;

            bool canAccept(const BaseType* other) const override;

            std::string getDebugName() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<AutoType>(*this); }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            ~AutoType() override = default;
    };

}

#endif
