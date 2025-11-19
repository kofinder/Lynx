/**
 * @file SanitizeMemoryHandler.hpp
 * @brief Handler that applies memory sanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Intended to mark functions for memory sanitization checks.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_SANITIZE_MEMORY_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_MEMORY_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class SanitizeMemoryHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                // Simple heuristic: any store, load, or alloca triggers memory sanitizer
                bool requiresMSan = false;
                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        if (llvm::isa<llvm::AllocaInst>(&I) ||
                            llvm::isa<llvm::LoadInst>(&I) ||
                            llvm::isa<llvm::StoreInst>(&I)) {
                            requiresMSan = true;
                            break;
                        }
                    }
                    if (requiresMSan) break;
                }
        
                if (requiresMSan) {
                    builder.addStringAttribute("sanitize_memory");
                    LOG_INFO("Applied 'sanitize_memory' attribute to function {}", func->getName().str());
                }       

            }
    };
            
}

#endif
