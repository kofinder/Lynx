/**
 * @file FastCallingConvSpec.hpp
 * @brief Defines a specification for detecting the LLVM Fast calling convention.
 *
 * @responsibilities
 * - Detects LLVM functions using the `Fast` calling convention.
 * - Maps functions matching this convention to `CallingConventionType::Fast`.
 * - Logs detection results to assist debugging and optimization tracing.
 *
 * @namespace LynxCallConv
 * Contains specifications that determine calling conventions for LLVM functions
 * within the Lynx compilation and runtime analysis framework.
 * Each specification checks for a specific LLVM calling convention type.
 * 
 * @note
 * The `Fast` calling convention attempts to minimize call overhead by passing
 * arguments in registers whenever possible for speed optimization.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_FAST_CALLING_CONV_SPEC_HPP
#define LYNX_FAST_CALLING_CONV_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class FastCallingConvSpec : public CallingConventionSpecification {

        public:
            bool isSatisfiedBy(llvm::Function* func) const override {
                // bool isEqual = func->hasFnAttribute("fast") && !func->isVarArg();
                // LOG_INFO("isSatisfiedBy {}", isEqual);

                bool isEqual = func->getCallingConv() == llvm::CallingConv::Fast;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override { return CallingConventionType::Fast; }

            ~FastCallingConvSpec() override = default;

    };
}


#endif 
