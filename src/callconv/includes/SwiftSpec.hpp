/**
 * @file SwiftSpec.hpp
 * @brief Defines a specification for detecting LLVM functions using the Swift calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions declared with the `Swift` calling convention.
 * - Maps the detected convention to `CallingConventionType::Swift` within the Lynx compiler.
 * - Enables interoperability and proper code generation for Swift-compatible functions.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify LLVM calling conventions and map them
 * to Lynx’s internal calling convention types.
 *
 * @note
 * The Swift calling convention is used to interoperate between C++/LLVM and Swift-generated code,
 * allowing efficient parameter passing according to Swift ABI conventions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_SWIFT_SPEC_HPP
#define LYNX_CALLING_CONV_SWIFT_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class SwiftSpec : public CallingConventionSpecification {
        
        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::Swift;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::Swift; }

            ~SwiftSpec() override = default;

    };
}


#endif 
