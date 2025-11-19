/**
 * @file SanitizeAddressHandler.hpp
 * @brief Handler that applies AddressSanitizer attributes to LLVM functions.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Intended to mark functions for AddressSanitizer instrumentation.
 * - Currently a placeholder; actual attribute application can be implemented as needed.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_SANITIZE_ADDRESS_HANDLER_HPP
#define LYNX_FUNC_SANITIZE_ADDRESS_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class SanitizeAddressHandler : public FunctionAttributeHandler {

        protected:

            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                bool requiresASan = false; 

                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        if (llvm::isa<llvm::StoreInst>(&I)) {
                            requiresASan = true;
                            break;
                        }
        
                        if (llvm::isa<llvm::GetElementPtrInst>(&I)) {
                            requiresASan = true;
                            break;
                        }
        
                        if (const auto *call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                            if (call->getCalledFunction()) {
                                llvm::StringRef name = call->getCalledFunction()->getName();
                                if (name.contains("memcpy") || name.contains("memset") || name.contains("memmove")) {
                                    requiresASan = true;
                                    break;
                                }
                            }
                        }
                    }

                    if (requiresASan) break;
                }
        
                if (requiresASan) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), "sanitize_address"));
                    LOG_INFO("Applied 'sanitize_address' attribute to function {}", func->getName().str());
                }
            }
    };    

}

#endif
