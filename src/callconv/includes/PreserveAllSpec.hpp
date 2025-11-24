/**
 * @file PreserveAllSpec.hpp
 * @brief Defines a specification for detecting functions using the LLVM PreserveAll calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions that use the `PreserveAll` calling convention.
 * - Provides internal mapping to `CallingConventionType::PreserveAll`.
 * - Facilitates convention-aware optimizations and verification during IR analysis.
 *
 * @namespace LynxCallConv
 * Contains specifications responsible for detecting and classifying LLVM calling conventions
 * within the Lynx compiler infrastructure.
 *
 * @note
 * The `PreserveAll` calling convention preserves all registers for the caller,
 * ensuring maximum safety during interprocedural function calls.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_PRESERVE_ALL_SPEC_HPP
#define LYNX_CALLING_CONV_PRESERVE_ALL_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class PreserveAllSpec : public CallingConventionSpecification {
        
        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::PreserveAll;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::PreserveAll; }

            ~PreserveAllSpec() override = default;

    };
        
        
}

#endif 
