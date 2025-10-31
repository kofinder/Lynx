#ifndef LYNX_FUNC_UW_TABLE_HANDLER_HPP
#define LYNX_FUNC_UW_TABLE_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class UWTableHandler : public FunctionAttributeHandler {
        
        private:

            bool hasLandingPad(const llvm::Function* func) {
                for (const auto& BB : *func) {
                    if (llvm::isa<llvm::LandingPadInst>(BB.getFirstNonPHI())) {
                        return true;
                    }    
                }
                return false;
            }
            
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked UWTableHandler");
                if (!func->hasFnAttribute(llvm::Attribute::NoUnwind) || hasLandingPad(func)) {
                    builder.addAttribute(llvm::Attribute::UWTable);
                    LOG_ERROR("Applied UWTable attributes");
                }
            }
        };              

}

#endif
