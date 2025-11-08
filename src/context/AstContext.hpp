#ifndef LYNX_AST_CONTEXT_HPP
#define LYNX_AST_CONTEXT_HPP 

/**
 * @file LynxAstContext.hpp
 * @brief Declares the AstContext class for managing AST types, 
 * LLVM context, and utilities for syntax tree operations.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 3, 2024
 */

#include <iostream>  
#include <string>
#include <stack>  
#include <vector>  
#include <map>  
#include <memory>
#include <logger/Logger.hpp>
#include "VirtualTable.hpp"
#include <llvm/IR/DIBuilder.h>
#include <constants/DataType.hpp>
#include <errors/LynxError.hpp>
#include <types/interfaces/BaseType.hpp>
#include <constants/VariableType.hpp>
#include <errors/includes/CompositeError.hpp>

namespace LynxContext {

    using namespace LynxTypes;
    using namespace LynxErrors;
    using namespace LynxConstants;
    
    class GlobalSymbolContext;

    class AstContext {  

        private: 

            std::shared_ptr<GlobalSymbolContext> globalContext;

            std::shared_ptr<llvm::LLVMContext> llvmContext;

            std::shared_ptr<llvm::IRBuilder<>> builder;

            std::shared_ptr<llvm::Module> module;

            llvm::DataLayout dataLayout;

            std::shared_ptr<llvm::DIBuilder> debugBuilder;

            std::shared_ptr<CompositeError> errors;

            std::shared_ptr<std::map<std::string, std::shared_ptr<BaseType>>> types;

        public:  

            // Tracks the current depth level of AST nodes within this context
            int currentDepth;

            // Constructor: Initializes core components of the AST context
            AstContext(const std::string& moduleName, 
                std::shared_ptr<llvm::LLVMContext> llvmContext, 
                std::shared_ptr<GlobalSymbolContext> sharedSymbols,
                std::shared_ptr<std::map<std::string, std::shared_ptr<BaseType>>> types
            );

            /**
             * @brief Creates a new AST context for nested scopes, inheriting from the current one.
             * @return Shared pointer to the newly created AstContext.
             */
            std::shared_ptr<AstContext> createContext() const;  

            /**
             * @brief Initializes default types within the context's type registry.
            */
            void initializeDefaultTypes();  

            /**
             * @brief Searches for a type by its name within the context's type registry.
             * 
             * Looks up the BaseType instance registered under the given type name.
             * 
             * @param name The name of the type to search for.
             * @return Shared pointer to the BaseType if found; nullptr if the type does not exist.
             */
            std::shared_ptr<BaseType> findType(const std::string& name) const;

            /**
             * @brief Retrieves a BaseType instance corresponding to the given VariableType enum.
             * 
             * Maps the VariableType enum value to its associated BaseType.
             * 
             * @param variableType The enumerated variable type.
             * @return Shared pointer to the BaseType associated with the enum; nullptr if not found.
             */
            std::shared_ptr<BaseType> findType(DataType variableType) const;

            /**
             * @brief Retrieves a BaseType instance for the given VariableType structure.
             * 
             * Handles lookup using either the VariableType or the custom name stored in VariableType.
             * 
             * @param varType The VariableType structure containing type information.
             * @return Shared pointer to the BaseType; nullptr if the type cannot be found.
             */
            std::shared_ptr<BaseType> findType(VariableType varType) const;

            /**
             * @brief Retrieves the BaseType instance corresponding to the LLVM value's type.
             * 
             * Inspects the LLVM value's type (removing pointer indirection) and matches it against
             * registered BaseTypes.
             * 
             * @param llvmValue The LLVM IR value whose type is to be resolved.
             * @return Shared pointer to the matching BaseType.
             * @throws std::runtime_error If no matching type is found.
             */
            std::shared_ptr<BaseType> findType(llvm::Value* llvmValue) const;

            /**
             * @brief Registers a custom type into the context's type registry.
             * @param name The name identifier for the type.
             * @param type The BaseType object to register.
             * @return true if the type was successfully registered; false if it already exists.
             */
            bool registerCustomType(const std::string& name, std::shared_ptr<BaseType> type); 

            /**
             * @brief Retrieves the global symbol context.
             * @return Shared pointer to the GlobalSymbolContext.
            */
            std::shared_ptr<GlobalSymbolContext> getGlobalContext() const;

            /**
             * @brief Provides access to the underlying LLVM context.
             * @return Reference to the LLVMContext.
            */
            llvm::LLVMContext& getLLVMContext() const { return *llvmContext; }

            /**
             * @brief Provides access to the LLVM IRBuilder for generating instructions.
             * @return Reference to the IRBuilder.
            */
            llvm::IRBuilder<>& getBuilder() const { return *builder; }

            /**
             * @brief Retrieves the current LLVM module.
             * @return Pointer to the LLVM module.
            */
            llvm::Module* getModule() const { return module.get(); }  

            /**
             * @brief Provides access to the LLVM debug information builder.
             * @return Reference to the DIBuilder.
            */
            llvm::DIBuilder& getDebugBuilder() const { return *debugBuilder; }

            /**
             * @brief Retrieves the data layout used for target-specific type sizes and alignments.
             * @return Constant reference to the DataLayout object.
            */
            const llvm::DataLayout& getDataLayout() const { return dataLayout; }

            /**
             * @brief Emits an LLVM IR call to allocate a GC-managed object.
             * 
             * Generates a call to the appropriate GC allocation function for the given
             * object type and returns a properly typed pointer in LLVM IR.
             *
             * @param objType The LLVM type of the object to allocate.
             * @return llvm::Value* Pointer to the newly allocated object (typed).
            */
            llvm::Value* emitGCAllocCall(llvm::Type* objType, std::string objectName);

            /**
             * @brief Retrieves or inserts a GC allocation function into the current LLVM module.
             * 
             * If the function does not exist in the module, it is created with the given
             * allocation size and name. Otherwise, the existing function is returned.
             *
             * @param allocSize Constant integer representing the size of the object in bytes.
             * @param fnName Optional name for the allocation function (default: "LYNX_GC_ALLOC").
             * @return llvm::Function* Pointer to the GC allocation function.
            */
            llvm::Function* getOrInsertGCAllocFunc(llvm::ConstantInt* allocSize, const std::string& fnName = "LYNX_GC_ALLOC");

            /**
             * @brief Reports an error into the context's error collection.
             * @param err The error object to report.
            */
            void reportError(std::shared_ptr<LynxError> err);

            /**
             * @brief Logs all accumulated errors to the standard error stream or logger.
            */
            void logErrors();

            ~AstContext() = default;
    };  
}

#endif
