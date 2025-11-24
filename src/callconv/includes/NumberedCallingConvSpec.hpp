/**
 * @file NumberedCallingConvSpec.hpp
 * @brief Defines a specification for detecting LLVM functions with custom numbered calling conventions.
 *
 * @responsibilities
 * - Detects LLVM functions that use a user-defined or non-standard numeric calling convention.
 * - Maps such detections to the internal `CallingConventionType::Numbered` type for classification.
 * - Supports analysis and debugging of platform-specific or experimental calling conventions in LLVM IR.
 *
 * @namespace LynxCallConv
 * Contains specifications responsible for detecting and classifying LLVM calling conventions
 * within the Lynx compiler infrastructure.
 *
 * @note
 * LLVM allows assigning custom numeric calling conventions (e.g., `CallingConv::ID = 99`).
 * This specification helps identify such functions and record their numeric identifiers.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_NUMBER_CALL_SPEC_HPP
#define LYNX_CALLING_CONV_NUMBER_CALL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class NumberedCallingConvSpec : public CallingConventionSpecification {

        unsigned ccNum;

        public:
        
            NumberedCallingConvSpec() : ccNum(0) {}

            explicit NumberedCallingConvSpec(unsigned num) : ccNum(num) {}
        
            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == ccNum;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return false;
            }
        
            CallingConventionType getCallingConvention() const override { return CallingConventionType::Numbered; }

            ~NumberedCallingConvSpec() override = default;

        };
    
}


#endif 
