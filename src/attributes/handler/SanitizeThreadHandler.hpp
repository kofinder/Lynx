/**
 * @file SanitizeThreadHandler.hpp
 * @brief Handler that applies thread sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for thread sanitization checks.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_SANITIZE_THREAD_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_THREAD_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class SanitizeThreadHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {

                if (!func) return;
        
                // Basic heuristic: atomic instructions or thread-unsafe calls
                bool requiresTSan = false;
                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        if (I.isAtomic()) {
                            requiresTSan = true;
                            break;
                        }
        
                        if (const auto *call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                            if (call->getCalledFunction()) {
                                llvm::StringRef name = call->getCalledFunction()->getName();
                                if (name.contains("pthread") || name.contains("std::mutex")) {
                                    requiresTSan = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (requiresTSan) break;
                }
        
                if (requiresTSan) {
                    builder.addStringAttribute("sanitize_thread");
                    LOG_INFO("Applied 'sanitize_thread' attribute to function {}", func->getName().str());
                }
            }

    };        
        
}

#endif
