/**
 * @file CFGuardCheckSpec.hpp
 * @brief Defines a specification for detecting the `CFGuard_Check` calling convention in LLVM functions.
 *
 * @responsibilities
 * - Determines whether a function uses the `CFGuard_Check` calling convention.
 * - Provides the associated `CallingConventionType` when the condition is met.
 * - Logs detection results for debugging and verification.
 *
 * @namespace LynxCallConv
 * Provides rule-based logic for identifying and classifying calling conventions within LLVM IR functions.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_CF_GURD_SPEC_HPP
#define LYNX_CALLING_CONV_CF_GURD_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {
    
    using namespace LynxConstants;

    class CFGuardCheckSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::CFGuard_Check;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::CFGuardCheck; }

            ~CFGuardCheckSpec() override = default;

        };
}


#endif 
