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

#include "interfaces/FunctionAttributeHandler.hpp"
#include <logger/Logger.hpp>

namespace LynxFunctionAttr {

    using namespace LynxLogger;


    class SanitizeAddressHandler : public FunctionAttributeHandler {
        protected:
            void apply(llvm::Function* func, FunctionAttributeBuilder& builder) override {
                // LOG_INFO("Invoked SanitizeAddressHandler");
                if (false) {
                    builder.addAttribute(llvm::Attribute::SanitizeAddress);
                    LOG_INFO("Applied SanitizeAddres attribues");
                }
            }
    };
        

}

#endif
