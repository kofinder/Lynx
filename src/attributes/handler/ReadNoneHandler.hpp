/**
 * @file ReadNoneHandler.hpp
 * @brief Handler that applies the LLVM ReadNone attribute to pure, side-effect-free functions.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Detects functions that neither read nor write memory.
 * - Marks such functions with the LLVM `ReadNone` attribute to enable aggressive optimizations.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_READ_NONE_HANDLER_HPP
#define LYNX_FUNC_READ_NONE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class ReadNoneHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;

                bool isPureAndSideEffectFree = true;

                for (const auto &BB : *func) {
                    for (const auto &I : BB) {
                        if (I.mayWriteToMemory() || I.mayHaveSideEffects()) {
                            isPureAndSideEffectFree = false;
                            break;
                        }
                    }
                    if (!isPureAndSideEffectFree) break;
                }
        
                if (isPureAndSideEffectFree) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), llvm::Attribute::ReadNone));
                    LOG_WARN("Applied 'readnone' attribute to function {}", func->getName().str());
                }
            }    
    };    

}

#endif
