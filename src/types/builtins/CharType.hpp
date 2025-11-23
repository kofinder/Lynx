/**
 * @file CharType.hpp
 * @brief Defines the CharType class representing the built-in character type in the Lynx type system.
 *
 * The `CharType` models a single Unicode-compatible character (typically an 8-bit or 16-bit code unit)
 * within the Lynx language. It provides functionality for LLVM IR generation, character manipulation,
 * and integration with the debug metadata system.
 *
 * **Key Responsibilities:**
 * - Represents a single character constant or variable.
 * - Maps to the appropriate LLVM integer type (e.g., `i8` or `i16`) depending on the target platform.
 * - Provides support for assignment, default value creation, and type casting to string pointers.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic and codegen phases.
 * - Generates DWARF-compatible debug metadata for character types.
 *
 * **Integration Points:**
 * - Used in string literals, text processing, and character-level operations.
 * - Commonly appears in arrays and sequences (`char[]` → string).
 * - Interoperates with `StringType` via `castToStringPointer()`.
 *
 * **LLVM Details:**
 * - Typically maps to `llvm::Type::getInt8Ty()` or `llvm::Type::getInt16Ty()`.
 * - Default value: zero-initialized (`'\0'`).
 * - Provides pointer type and size/alignment via target `DataLayout`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_CHAR_TYPE_HPP
#define LYNX_CHAR_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class CharType : public BuiltInType {

        private:
        
            static llvm::StructType* cachedType;

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit CharType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            CharType(const CharType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~CharType() override = default;
            CharType& operator=(const CharType&) = delete;
            CharType(CharType&&) = delete;
            CharType& operator=(CharType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<CharType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* createValue(LValueType value) const override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            void accept(TypeVisitor& visitor) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return charMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;
            
            llvm::Value* castToStringPointer(llvm::Value* arrayValue);

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::CHAR; }

            std::string getDebugName() const override { return "char"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif 
