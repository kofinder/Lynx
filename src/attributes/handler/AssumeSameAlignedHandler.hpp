/**
 * @file AssumeSameAlignedHandler.hpp
 * @brief Handler for applying LLVM "assume same alignment" related attributes.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers for LLVM functions in the Lynx compiler.
 * - Intended to mark that certain arguments or memory accesses have the same alignment.
 * - Prepares functions for LLVM optimization passes that can leverage aligned memory assumptions.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNC_ASSUME_SAME_ALIGNED_HANDLER_HPP
#define LYNX_FUNC_ASSUME_SAME_ALIGNED_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class AssumeSameAlignedHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                //LOG_INFO("Invoked AssumeSameAlignedHandler");
            }
    };
        
}

#endif
