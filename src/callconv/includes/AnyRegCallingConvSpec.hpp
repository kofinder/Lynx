/**
 * @file AnyRegCallingConvSpec.hpp
 * @brief Defines a specification for detecting the `AnyReg` calling convention in LLVM functions.
 *
 * @responsibilities
 * - Determines whether an LLVM function uses the `AnyReg` calling convention.
 * - Provides the associated `CallingConventionType` if the specification is satisfied.
 * - Logs the evaluation result for debugging and verification purposes.
 *
 * @namespace LynxCallConv
 * Contains rule-based specifications for identifying calling conventions in LLVM IR functions.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_ANY_REG_SPEC_HPP
#define LYNX_CALLING_CONV_ANY_REG_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"
#include <logger/Logger.hpp>


namespace LynxCallConv {

    using namespace LynxLogger;
    using namespace LynxConstants;
    
    class AnyRegCallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::AnyReg;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::AnyReg; }

            ~AnyRegCallingConvSpec() override = default;

    };      
        
}


#endif 
