/**
 * @file PreserveNoneSpec.hpp
 * @brief Defines a specification for detecting functions using the LLVM PreserveNone calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions declared with the `PreserveNone` calling convention.
 * - Maps recognized functions to the internal `CallingConventionType::PreserveNone`.
 * - Provides a structure for future support of functions with no preserved registers.
 *
 * @namespace LynxCallConv
 * Contains specifications responsible for detecting and classifying LLVM calling conventions
 * within the Lynx compiler infrastructure.
 *
 * @note
 * The `PreserveNone` calling convention indicates that no registers are preserved across calls,
 * making it suitable for lightweight, low-level system routines.
 * This detection is currently disabled and returns `false` until implemented in LLVM IR.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_PRESERVE_NONE_SPEC_HPP
#define LYNX_CALLING_CONV_PRESERVE_NONE_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class PreserveNoneSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                //return func->getCallingConv() == llvm::CallingConv::PreserveNone;
                LOG_INFO("isSatisfiedBy {}", false);
                return false;
            }

            CallingConventionType getCallingConvention() const override {
                return CallingConventionType::PreserveNone;
            }

            ~PreserveNoneSpec() override = default;

        };
         
        
}

#endif 
