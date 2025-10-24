#ifndef LYNX_FUNCTION_ATTRS_INFERER_HPP
#define LYNX_FUNCTION_ATTRS_INFERER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/LLVMContext.h>
#include "FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    /**
     * @brief Central orchestrator that runs a chain of attribute inference handlers on LLVM functions.
     *
     * This class initializes and manages a chain of `FunctionAttributeHandler` instances.
     * It provides a single entry point to infer and apply various safe or optimized function attributes
     * to an LLVM `Function`, based on the handlers registered in the chain.
     */

    class FunctionAttributeInferer {
        /// Head of the chain of attribute handlers.
        std::unique_ptr<FunctionAttributeHandler> head;
    
    public:
        
        /**
         * @brief Constructor that sets up the chain of attribute handlers.
         *
         * Typically, this initializes the `head` with a default or configured set of attribute logic
         * (e.g., intrinsic checking, parameter attribute inference, etc.).
         */
        FunctionAttributeInferer();

        /**
         * @brief Entry point to infer and apply attributes to the given LLVM function.
         * 
         * This will invoke the chain of handlers, each applying its own attribute logic
         * to the provided function via a `FunctionAttributeBuilder`.
         *
         * @param func The LLVM function on which to infer and apply attributes.
         */
        void inferAndApply(llvm::Function* func);
    
        ~FunctionAttributeInferer();

    };
    
}

#endif
