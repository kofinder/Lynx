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
#include "llvm/IR/Attributes.h"
#include "FunctionAttributeHandler.hpp"

#include "handler/SafeDefaultsHandler.hpp"
#include "handler/AlwaysInlineHandler.hpp"
#include "handler/NoInlineHandler.hpp"
#include "handler/NoReturnHandler.hpp"
#include "handler/ReadOnlyHandler.hpp"
#include "handler/ReadNoneHandler.hpp"
#include "handler/NoCaptureHandler.hpp"
#include "handler/NoUnwindHandler.hpp"
#include "handler/SanitizeAddressHandler.hpp"
#include "handler/OptimizeForSizeHandler.hpp"
#include "handler/InlineHintHandler.hpp"
#include "handler/StackProtectorHandler.hpp"
#include "handler/NoInlineHintHandler.hpp"
#include "handler/NoDeadStripHandler.hpp"
#include "handler/IntrinsicHandler.hpp"
#include "handler/NoRecurseHandler.hpp"
#include "handler/SanitizeThreadHandler.hpp"
#include "handler/SanitizeMemoryHandler.hpp"
#include "handler/SanitizeHWAddressHandler.hpp"
#include "handler/NoInlineAsmHandler.hpp"
#include "handler/UWTableHandler.hpp"
#include "handler/ZeroExtendHandler.hpp"
#include "handler/SignExtendHandler.hpp"
#include "handler/ArgMemOnlyHandler.hpp"
#include "handler/ReturnsTwiceHandler.hpp"
#include "handler/StrictFPHandler.hpp"
#include "handler/SanitizeCoverageHandler.hpp"
#include "handler/AssumeSameAlignedHandler.hpp"
#include "handler/NoSanitizeMemoryHandler.hpp"
#include "handler/NoSanitizeThreadHandler.hpp"
#include "handler/NoSanitizeAddressHandler.hpp"
#include "handler/DereferenceableParamHandler.hpp"

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
        FunctionAttributeInferer() {
            auto headHandler = std::make_unique<SafeDefaultsHandler>();
            FunctionAttributeHandler* current = headHandler.get();

            auto append = [&](std::unique_ptr<FunctionAttributeHandler> next) {
                current->setNext(std::move(next));
                current = current->getNext(); // assumes getNext() returns raw ptr for chaining
            };

            append(std::make_unique<AlwaysInlineHandler>());
            append(std::make_unique<NoInlineHandler>());
            append(std::make_unique<NoReturnHandler>());
            append(std::make_unique<ReadOnlyHandler>());
            append(std::make_unique<ReadNoneHandler>());
            append(std::make_unique<NoCaptureHandler>());
            append(std::make_unique<NoUnwindHandler>());
            append(std::make_unique<SanitizeAddressHandler>());
            append(std::make_unique<OptimizeForSizeHandler>());
            append(std::make_unique<InlineHintHandler>());
            append(std::make_unique<StackProtectorHandler>());
            append(std::make_unique<NoInlineHintHandler>());
            append(std::make_unique<NoDeadStripHandler>());
            append(std::make_unique<IntrinsicHandler>());
            append(std::make_unique<NoRecurseHandler>());
            append(std::make_unique<SanitizeThreadHandler>());
            append(std::make_unique<SanitizeMemoryHandler>());
            append(std::make_unique<SanitizeHWAddressHandler>());
            append(std::make_unique<NoInlineAsmHandler>());
            append(std::make_unique<UWTableHandler>());
            append(std::make_unique<ZeroExtendHandler>());
            append(std::make_unique<SignExtendHandler>());
            append(std::make_unique<ArgMemOnlyHandler>());
            append(std::make_unique<ReturnsTwiceHandler>());
            append(std::make_unique<StrictFPHandler>());
            append(std::make_unique<SanitizeCoverageHandler>());
            append(std::make_unique<AssumeSameAlignedHandler>());
            append(std::make_unique<NoSanitizeMemoryHandler>());
            append(std::make_unique<NoSanitizeThreadHandler>());
            append(std::make_unique<NoSanitizeAddressHandler>());
            append(std::make_unique<DereferenceableParamHandler>());

            head = std::move(headHandler);
        }

        /**
         * @brief Entry point to infer and apply attributes to the given LLVM function.
         * 
         * This will invoke the chain of handlers, each applying its own attribute logic
         * to the provided function via a `FunctionAttributeBuilder`.
         *
         * @param func The LLVM function on which to infer and apply attributes.
         */
        void inferAndApply(llvm::Function* func) {
            FunctionAttributeBuilder builder(func);
            if (head) {
                head->handle(func, builder);
            }
            builder.apply();
        }
    
        ~FunctionAttributeInferer() noexcept = default;
    };
    
}

#endif
