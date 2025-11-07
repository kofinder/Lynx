/**
 * @file FunctionAttributeInferer.hpp
 * @brief Declares the FunctionAttributeInferer class for automated LLVM function attribute inference.
 * 
 * The `FunctionAttributeInferer` class manages a chain of `FunctionAttributeHandler` instances
 * to automatically analyze, infer, and apply LLVM function attributes such as:
 * - `AlwaysInline`, `NoUnwind`, or `ReadNone` for optimization.
 * - `NonNull`, `NoAlias` for return and parameter safety.
 * 
 * This system enables **automated function attribute inference** by composing handlers
 * that each encapsulate specific rules. The inferer initializes and triggers the handler chain,
 * ensuring attributes are applied consistently and efficiently across generated LLVM functions.
 * 
 * @see FunctionAttributeHandler
 * @see FunctionAttributeBuilder
 * @see llvm::Function
 * 
 * @namespace LynxFunctionAttr
 * Provides builder and inference utilities for LLVM function attributes in Lynx.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNCTION_ATTRS_INFERER_HPP
#define LYNX_FUNCTION_ATTRS_INFERER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/LLVMContext.h>
#include "FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {


    class FunctionAttributeInferer {
        
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
