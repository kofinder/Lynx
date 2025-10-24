#ifndef LYNX_FUNCTION_ATTRS_HANDLER_HPP
#define LYNX_FUNCTION_ATTRS_HANDLER_HPP

#include <llvm/IR/Function.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/Value.h>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "FunctionAttributeBuilder.hpp"

namespace LynxFunctionAttr {

    /**
     * @brief Abstract base class for applying function attributes using a chain-of-responsibility pattern.
     *
     * This class allows you to define handlers that apply specific LLVM attributes to functions
     * using a customizable and composable pipeline. Each handler can perform an operation and
     * delegate to the next in the chain.
     */

    class FunctionAttributeHandler {

        protected:

            /// Pointer to the next handler in the chain
            std::unique_ptr<FunctionAttributeHandler> next = nullptr;

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
