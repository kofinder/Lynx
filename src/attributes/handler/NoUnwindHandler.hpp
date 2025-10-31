#ifndef LYNX_FUNC_NO_UNWIND_HANDLER_HPP
#define LYNX_FUNC_NO_UNWIND_HANDLER_HPP


#include "interfaces/FunctionAttributeHandler.hpp"

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
                // LOG_INFO("Invoked NoUnwindHandler");
                if (!mayThrow(func)) {
                    LOG_ERROR("Applied NonUnwind attributes");
                    builder.addAttribute(llvm::Attribute::NoUnwind);
                }
                
            }
    
    };    

}

#endif
