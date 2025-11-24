/**
 * @file ThisCallSpec.hpp
 * @brief Defines a specification for detecting LLVM functions using the ThisCall calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions declared with the `X86_ThisCall` calling convention.
 * - Maps the detected convention to `CallingConventionType::ThisCall` within the Lynx compiler.
 * - Supports proper handling of member function calls on x86 architectures.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify LLVM calling conventions and map them
 * to Lynx’s internal calling convention types.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_THIS_CALL_SPEC_HPP
#define LYNX_CALLING_CONV_THIS_CALL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class ThisCallSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::X86_ThisCall;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::ThisCall; }

            ~ThisCallSpec() override = default;
        };
        
        
}


#endif 
