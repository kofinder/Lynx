/**
 * @file StringType.hpp
 * @brief Defines the StringType class representing the built-in string type in the Lynx type system.
 *
 * The `StringType` encapsulates text data within the Lynx language.
 * It provides mechanisms for LLVM IR generation, semantic analysis, and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents sequences of characters (`String` in Lynx).
 * - Generates the corresponding LLVM IR type (typically a pointer to character array or custom string struct).
 * - Supports variable instantiation, assignment, and literal value creation.
 * - Integrates with `TypeVisitor` and `TypeMethodResolver` for semantic checks and method resolution.
 * - Produces DWARF-compatible debug information for source-level debugging.
 *
 * **Integration Points:**
 * - Used in string concatenation, printing, comparisons, and indexing.
 * - Interoperates with `CharType` for single-character operations.
 * - Default initialization yields an empty string (`""`).
 *
 * **LLVM Details:**
 * - Maps to LLVM representation for string storage (e.g., `i8*` or custom struct pointer).
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Default LLVM value: typically a null pointer or empty string constant.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_STRING_TYPE_HPP
#define LYNX_STRING_TYPE_HPP

#include <types/interfaces/BuiltInType.hpp>

namespace LynxTypes {

    class StringType : public BuiltInType {

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit StringType(AstContext* context) : BuiltInType(context) {}

            // Public copy constructor, needed for clone()
            StringType(const StringType& other) : BuiltInType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~StringType() override = default;
            StringType& operator=(const StringType&) = delete;
            StringType(StringType&&) = delete;
            StringType& operator=(StringType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<StringType>(*this);
            }

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* createValue(LValueType value) const override;
            
            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            TypeMethodResolver* getOrCreateResolver() const  override;

            void accept(TypeVisitor& visitor) override;
            
            const std::unordered_map<std::string_view, int>& getMethodRegistry() const override { return stringMethods; }

            llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            bool equals(const BaseType* other) const override;

            DataType getTypeTag() const override { return DataType::STRING; }

            std::string getDebugName() const override { return "string"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif 
