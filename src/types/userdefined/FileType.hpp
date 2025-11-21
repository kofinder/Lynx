/**
 * @file FileType.hpp
 * @brief Defines the FileType class representing the built-in file type in the Lynx type system.
 *
 * The `FileType` encapsulates file handles and operations for working with files
 * in the Lynx language. It provides mechanisms for LLVM IR generation, semantic analysis,
 * and debug metadata emission.
 *
 * **Key Responsibilities:**
 * - Represents file objects used for reading, writing, and managing files.
 * - Supports variable instantiation, assignment, and default value creation.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for file types.
 *
 * **Integration Points:**
 * - Used in file I/O operations such as open, read, write, and close.
 * - Interacts with system-level or standard library APIs for file management.
 * - Default initialization represents an invalid or closed file handle.
 *
 * **LLVM Details:**
 * - Maps to `llvm::StructType` for representing the file handle or file object.
 * - Provides pointer type and size/alignment according to target `DataLayout`.
 * - Maintains a static cached LLVM type for performance.
 *
 * **Additional Features:**
 * - Provides utilities for type comparison, cloning, and debug name resolution.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_FILE_TYPE_HPP
#define LYNX_FILE_TYPE_HPP

#include <types/interfaces/UserDefinedType.hpp>

namespace LynxTypes {

    class FileType : public UserDefinedType {

        private:
        
            static llvm::StructType* cachedType;

        protected:

            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            explicit FileType(AstContext* context) : UserDefinedType(context) {}

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            llvm::Value* createInstance(std::string variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;
            
            // void accept(TypeVisitor& visitor) override;

            // TypeMethodResolver* getOrCreateResolver() const  override;

            // const std::unordered_map<std::string_view, int>& getMethodRegistry() const override;

            // const std::unordered_map<std::string, int>& getInstanceMethodRegistry() const override;

            // llvm::Value* emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) override;

            bool equals(const BaseType* other) const override;

            inline DataType getTypeTag() const override { return DataType::FILE; }

            std::string getDebugName() const override { return "File"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;

            std::unique_ptr<BaseType> clone() const override { return std::make_unique<FileType>(*this); }

            ~FileType() override = default;
    };
}
#endif 
