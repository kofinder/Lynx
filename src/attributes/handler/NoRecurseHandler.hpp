#ifndef LYNX_FUNC_NO_RECURSE_HANDLER_HPP
#define LYNX_FUNC_NO_RECURSE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

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
