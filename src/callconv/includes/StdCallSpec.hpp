/**
 * @file StdCallSpec.hpp
 * @brief Defines a specification for detecting functions using the LLVM StdCall calling convention.
 *
 * @responsibilities
 * - Detects LLVM functions declared with the `X86_StdCall` calling convention.
 * - Associates detected functions with the internal `CallingConventionType::StdCall`.
 * - Enables Lynx to handle Windows-specific calling conventions correctly during code generation.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify LLVM calling conventions and map them to
 * Lynx’s internal calling convention types.
 *
 * @note
 * The `StdCall` convention is primarily used on 32-bit Windows systems.
 * It dictates that the callee cleans up the stack, commonly used in WinAPI functions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_STD_CALL_SPEC_HPP
#define LYNX_CALLING_CONV_STD_CALL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;


    class StdCallSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::X86_StdCall;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::StdCall; }

            ~StdCallSpec() override = default;

        };
}


#endif 
