/**
 * @file SwiftTailSpec.hpp
 * @brief Defines a specification for detecting LLVM functions using the SwiftTail calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions declared with the `SwiftTail` calling convention.
 * - Maps the detected convention to `CallingConventionType::SwiftTail` within the Lynx compiler.
 * - Enables tail-call optimizations and proper code generation for Swift interoperability.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify LLVM calling conventions and map them
 * to Lynx’s internal calling convention types.
 *
 * @note
 * The SwiftTail calling convention is used for Swift functions that support
 * tail-call optimization.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_SWIFT_TAIL_SPEC_HPP
#define LYNX_CALLING_CONV_SWIFT_TAIL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class SwiftTailSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::SwiftTail;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::SwiftTail; }

            ~SwiftTailSpec() override = default;
        };
        
}


#endif 
