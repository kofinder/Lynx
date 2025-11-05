/**
 * @file NoSanitizeAddressHandler.hpp
 * @brief Handler that manages functions that should not have address sanitizer attributes applied.
 *
 * @responsibilities
 * - Part of the function attribute handler chain in the Lynx compiler.
 * - Ensures that functions marked to skip address sanitization do not receive sanitizer attributes.
 *
 * @namespace LynxFunctionAttr
 * Contains classes that infer and apply LLVM function attributes in the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 5, 2025
*/

#ifndef LYNX_FUNC_NO_SANITIZE_ADDRESS_HANDLER_HPP
#define LYNX_FUNC_NO_SANITIZE_ADDRESS_HANDLER_HPP

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class NoSanitizeAddressHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked NoSanitizeAddressHandler");
            }
        };                
}

#endif
