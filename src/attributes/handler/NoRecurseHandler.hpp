/**
 * @file NoRecurseHandler.hpp
 * @brief Handler that applies NoRecurse attribute to functions that do not recurse.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Detects LLVM functions that do not call themselves (non-recursive).
 * - Applies the NoRecurse LLVM attribute to optimize non-recursive functions.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNC_NO_RECURSE_HANDLER_HPP
#define LYNX_FUNC_NO_RECURSE_HANDLER_HPP

#include "FunctionAttributeHandler.hpp"

#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;

    class NoRecurseHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoRecurseHandler");
                //Basic heuristic: no calls to self
                bool recurses = false;
                for (auto& bb : *func) {
                    for (auto& inst : bb) {
                        if (auto* call = llvm::dyn_cast<llvm::CallBase>(&inst)) {
                            if (call->getCalledFunction() == func) {
                                recurses = true;
                                break;
                            }
                        }
                    }
                    if (recurses) break;
                }
                if (!recurses) {
                    builder.addAttribute(llvm::Attribute::NoRecurse);
                }
            }
        };
        
}

#endif
