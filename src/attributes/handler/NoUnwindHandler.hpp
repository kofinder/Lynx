/**
 * @file NoUnwindHandler.hpp
 * @brief Handler that applies the LLVM NoUnwind attribute to functions that cannot throw exceptions.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Analyzes the function body to determine if it may throw.
 * - Applies the LLVM `NoUnwind` attribute to functions that are guaranteed not to throw.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_NO_UNWIND_HANDLER_HPP
#define LYNX_FUNC_NO_UNWIND_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class NoUnwindHandler : public FunctionAttributeHandler {

        private:

            bool mayThrow(const llvm::Function* func) const {
                if (func->isDeclaration())
                    return false; // No body = nothing to throw

                for (const llvm::BasicBlock& BB : *func) {
                    for (const llvm::Instruction& I : BB) {
                        if (const auto* call = llvm::dyn_cast<llvm::CallBase>(&I)) {
                            if (!call->doesNotThrow())
                                return true;
                        }
                    }
                }

                return false; // No throwing call found
            }

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                if (!mayThrow(func)) {
                    builder.addAttribute(llvm::Attribute::get(func->getContext(), llvm::Attribute::NoUnwind));
                    LOG_ERROR("Applied 'nounwind' attribute to function {}", func->getName().str());
                }        
            }
    
    };    

}

#endif
