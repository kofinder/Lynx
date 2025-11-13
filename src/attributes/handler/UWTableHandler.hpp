/**
 * @file UWTableHandler.hpp
 * @brief Handler that applies the UWTable attribute for functions that may unwind.
 *
 * @responsibilities
 * - Part of the chain of function attribute handlers in the Lynx compiler.
 * - Inspects LLVM functions to determine if they may participate in exception handling.
 * - Applies the UWTable attribute if the function does not have NoUnwind or contains a landing pad.
 * - Ensures correct exception unwinding table generation for LLVM codegen.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FUNC_UW_TABLE_HANDLER_HPP
#define LYNX_FUNC_UW_TABLE_HANDLER_HPP

#include "attributes/FunctionAttributeHandler.hpp"

namespace LynxFunctionAttr {

    class UWTableHandler : public FunctionAttributeHandler {
        
        private:

            bool hasLandingPad(const llvm::Function* func) {
                for (const auto& BB : *func) {
                    if (auto* LPI = llvm::dyn_cast<llvm::LandingPadInst>(BB.getFirstNonPHIIt())) {
                        if (LPI) return true;
                    }
                }
                return false;
            }
            
        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                if (!func) return;
                if (!func->hasFnAttribute(llvm::Attribute::NoUnwind) || hasLandingPad(func)) {
                    llvm::LLVMContext &ctx = func->getContext();
                    builder.addAttribute(llvm::Attribute::get(ctx, llvm::Attribute::UWTable));
                    LOG_INFO("Applied 'UWTable' attribute to function {}", func->getName().str());
                }
            }
    };              

}

#endif
