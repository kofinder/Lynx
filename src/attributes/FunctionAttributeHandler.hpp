/**
 * @file FunctionAttributeHandler.hpp
 * @brief Defines the base class for handling LLVM function attributes through a chain of responsibility.
 * 
 * The `FunctionAttributeHandler` class provides an extensible framework to apply 
 * various function-level attributes in LLVM using a **chain-of-responsibility pattern**.
 * 
 * Each handler in the chain can implement a specific type of attribute logic 
 * (e.g., optimization, safety, or calling convention attributes). 
 * The chain allows multiple handlers to apply attributes sequentially 
 * using a shared `FunctionAttributeBuilder`.
 * 
 * @see FunctionAttributeBuilder
 * @see llvm::Function
 * 
 * @note This design allows modular extension of LLVM function attribute logic
 *       without coupling or repetitive attribute configuration code.
 * 
 * @namespace LynxFunctionAttr
 * Provides builder and handler utilities for LLVM function attributes in Lynx.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_FUNCTION_ATTRS_HANDLER_HPP
#define LYNX_FUNCTION_ATTRS_HANDLER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Value.h>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "FunctionAttributeBuilder.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;

namespace LynxFunctionAttr {

    class FunctionAttributeHandler {

        protected:

            /// Pointer to the next handler in the chain
            std::unique_ptr<FunctionAttributeHandler> next = nullptr;

        private:
        
            /**
             * @brief Abstract method to apply specific logic for attribute modification.
             *
             * Subclasses must implement this to apply their specific attributes.
             *
             * @param func Pointer to the LLVM function being processed.
             * @param builder Reference to the attribute builder that accumulates modifications.
             */
            virtual void apply(llvm::Function* func, FunctionAttributeBuilder& builder) = 0;


        public:

            virtual ~FunctionAttributeHandler() = default;

            /**
             * @brief Set the next handler in the chain.
             * @param nextHandler Unique pointer to the next handler.
             */
            void setNext(std::unique_ptr<FunctionAttributeHandler> nextHandler) {
                next = std::move(nextHandler);
            }

            /**
             * @brief Execute the current handler's logic and pass control to the next.
             * @param func Pointer to the LLVM function.
             * @param builder Reference to the attribute builder.
             */
            void handle(llvm::Function* func, FunctionAttributeBuilder& builder) {
                apply(func, builder);
                if (next) next->handle(func, builder);
            }

            /**
             * @brief Retrieve the next handler in the chain.
             * @return Raw pointer to the next handler (if any).
             */
            FunctionAttributeHandler* getNext() const { return next.get(); }
    };
}

#endif 
