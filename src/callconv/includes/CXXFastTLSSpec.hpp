/**
 * @file CXXFastTLSSpec.hpp
 * @brief Defines a specification for detecting the C++ Fast TLS calling convention in LLVM functions.
 *
 * @responsibilities
 * - Identifies LLVM functions using the `CXX_FAST_TLS` calling convention.
 * - Associates such functions with `CallingConventionType::CXXFastTLS`.
 * - Logs detection results for debugging and optimization tracking.
 *
 * @namespace LynxCallConv
 * Provides specifications used to identify LLVM calling conventions for runtime and compilation analysis.
 * Each specification defines detection rules and maps them to a defined calling convention type.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_CXX_FAST_TL_SPEC_HPP
#define LYNX_CALLING_CONV_CXX_FAST_TL_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class CXXFastTLSSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::CXX_FAST_TLS;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::CXXFastTLS; }

            ~CXXFastTLSSpec() override = default;

        };
        
}

#endif 
