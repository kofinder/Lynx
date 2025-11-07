/**
 * @file RuntimeFunction.hpp
 * @brief Defines the abstract interface for all Lynx runtime functions.
 * 
 * The `RuntimeFunction` class serves as the base interface for every runtime-executable
 * function within the Lynx Runtime Library. Each derived class implements a specific
 * runtime feature (e.g., filesystem access, collection manipulation, etc.).
 * 
 * **Key Responsibilities:**
 * - Provide a unified interface for invoking runtime functions.
 * - Support execution with and without additional LLVM IR values.
 * - Facilitate dynamic code generation and runtime evaluation.
 * 
 * **Usage Example:**
 * @code
 * class FileOpenFunction : public RuntimeFunction {
 * public:
 *     llvm::Value* call(std::shared_ptr<AstContext> ctx,
 *                       std::vector<std::unique_ptr<ExpressionNode>> args) override {
 *         // Custom implementation...
 *     }
 * };
 * @endcode
 * 
 * @see RuntimeFunctionRegistry, RuntimeModule, RuntimeValue
 * 
 * @namespace LynxLibRuntime
 * Provides runtime-level functionality bindings between the Lynx AST and LLVM IR.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
 */


#ifndef LYNX_LIB_RUNTIME_FUNCTION_HPP
#define LYNX_LIB_RUNTIME_FUNCTION_HPP

#include <string>
#include <vector>
#include "RuntimeValue.hpp"
#include <logger/Logger.hpp>
#include <llvm/IR/Value.h>
#include <ast/ExpressionNode.hpp>
#include <context/AstContext.hpp>

using namespace LynxLogger;
using namespace LynxAst;
using namespace LynxContext;

namespace LynxLibRuntime {

    class RuntimeFunction {

        public:

            virtual ~RuntimeFunction() = default;

            /**
             * @brief Execute the runtime function without an optional LLVM IR value.
             *
             * @param astContext The current AST context of the function call.
             * @param args Vector of unique pointers to expression nodes representing the function arguments.
             * @return An LLVM IR Value pointer representing the result of execution.
             *
             * @note
             * This method can be used when no auxiliary LLVM value needs to be passed.
             */
            virtual llvm::Value* call(
                std::shared_ptr<AstContext> astContext,
                std::vector<std::unique_ptr<ExpressionNode>> args
            ) = 0;

            /**
             * @brief Execute the runtime function with an optional LLVM IR value.
             *
             * @param astContext The current AST context of the function call.
             * @param args Vector of unique pointers to expression nodes representing the function arguments.
             * @param optionalValue An optional LLVM IR Value pointer, can be nullptr if unused.
             * @return An LLVM IR Value pointer representing the result of execution.
             *
             * @note
             * This method allows passing additional LLVM IR information to the function,
             * which may be required by certain runtime functions for code generation or optimization.
             */
            virtual llvm::Value* call(
                std::shared_ptr<AstContext> astContext,
                std::vector<std::unique_ptr<ExpressionNode>> args,
                llvm::Value* optionalValue
            ) = 0;

    };

}

#endif
