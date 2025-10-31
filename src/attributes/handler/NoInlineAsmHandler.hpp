#ifndef LYNX_FUNC_NO_INLINE_ASM_HANDLER_HPP
#define LYNX_FUNC_NO_INLINE_ASM_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>
using namespace LynxLogger;

namespace LynxFunctionAttr {

    class NoInlineAsmHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoke NoInlineAsmHandler");
                if (func->hasFnAttribute("no-inline-asm")) {
                    //builder.addAttribute(llvm::Attribute::NoInlineAsm);
                }
            }
        };          

}

#endif
