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

    /**
     * @class RuntimeFunction
     * @brief Abstract base class representing a runtime-callable function.
     *
     * @details
     * Defines the interface for all runtime functions in the interpreter.
     * Runtime functions operate on AST-level arguments and perform logic 
     * during program execution, returning an LLVM IR value.
     *
     * This interface supports dynamic invocation of both native and module-defined functions.
     */
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
