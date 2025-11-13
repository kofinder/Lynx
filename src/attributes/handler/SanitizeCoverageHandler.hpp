/**
 * @file SanitizeCoverageHandler.hpp
 * @brief Handler that applies coverage sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for coverage instrumentation.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/


#ifndef LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_COVERAGE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class SanitizeCoverageHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                auto &ctx = func->getContext();
                
                bool requiresHWASan = false;
                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        // Detect memory stores
                        if (llvm::isa<llvm::StoreInst>(&I)) {
                            requiresHWASan = true;
                            break;
                        }
        
                        // Detect pointer arithmetic
                        if (llvm::isa<llvm::GetElementPtrInst>(&I)) {
                            requiresHWASan = true;
                            break;
                        }
        
                        // Detect stack memory allocations
                        if (llvm::isa<llvm::AllocaInst>(&I)) {
                            requiresHWASan = true;
                            break;
                        }
        
                        // Memory intrinsics: memcpy/memset/memmove
                        if (const auto *call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                            if (call->getCalledFunction()) {
                                llvm::StringRef name = call->getCalledFunction()->getName();
                                if (name.contains("memcpy") || name.contains("memset") || name.contains("memmove")) {
                                    requiresHWASan = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (requiresHWASan) break;
                }
        
                if (requiresHWASan) {
                    builder.addAttribute(llvm::Attribute::get(ctx, "sanitize_hwaddress"));
                    LOG_INFO("Applied 'sanitize_hwaddress' attribute to function {}", func->getName().str());
                }        
            }
    }; 
                
}

#endif
