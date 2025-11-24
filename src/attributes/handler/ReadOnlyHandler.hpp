/**
 * @file ReadOnlyHandler.hpp
 * @brief Handler that applies the LLVM ReadOnly attribute to functions that only read memory.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Detects functions that do not modify memory.
 * - Marks such functions with the LLVM `ReadOnly` attribute to aid optimization.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 5, 2025
*/


#ifndef LYNX_FUNC_READ_ONLY_HANDLER_HPP
#define LYNX_FUNC_READ_ONLY_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class ReadOnlyHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                bool readsMemoryOnly = true;
                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        if (I.mayWriteToMemory() || I.mayHaveSideEffects()) {
                            readsMemoryOnly = false;
                            break;
                        }
                    }
                    if (!readsMemoryOnly) break;
                }
        
                if (readsMemoryOnly && !func->onlyReadsMemory()) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), llvm::Attribute::ReadOnly));
                    LOG_WARN("Applied 'readonly' attribute to function {}", func->getName().str());
                }        
            }
    };    

}

#endif
