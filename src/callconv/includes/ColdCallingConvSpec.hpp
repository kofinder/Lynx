/**
 * @file ColdCallingConvSpec.hpp
 * @brief Defines a specification for detecting the `cold` calling convention in LLVM functions.
 *
 * @responsibilities
 * - Identifies LLVM functions marked with the `cold` attribute.
 * - Provides the `CallingConventionType::Cold` classification for such functions.
 * - Logs detection results for diagnostic and optimization analysis.
 *
 * @namespace LynxCallConv
 * Contains specifications and rules for detecting different LLVM calling conventions
 * within the Lynx compiler runtime infrastructure.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_CALLING_CONV_COLD_SPEC_HPP
#define LYNX_CALLING_CONV_COLD_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {
    
    using namespace LynxConstants;

    class ColdCallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->hasFnAttribute("cold");
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override {  return CallingConventionType::Cold; }

            ~ColdCallingConvSpec() override = default;

    };
}


#endif 
