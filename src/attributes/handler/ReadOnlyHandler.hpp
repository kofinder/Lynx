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
 * @date: November 5, 2025
*/


#ifndef LYNX_FUNC_READ_ONLY_HANDLER_HPP
#define LYNX_FUNC_READ_ONLY_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class ReadOnlyHandler : public FunctionAttributeHandler {

        protected:
        
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked ReadOnlyHandler");
                if (/* check if function only reads memory */ false) {
                    builder.addAttribute(llvm::Attribute::ReadOnly);
                    LOG_WARN("Applied readonly attributes");
                }
            }
    
    };    

}

#endif
