/**
 * @file TailSpec.hpp
 * @brief Defines a specification for detecting LLVM functions using the Tail calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions declared with the `Tail` calling convention.
 * - Maps the detected convention to `CallingConventionType::Tail` within the Lynx compiler.
 * - Supports tail-call optimization and proper code generation for functions using this convention.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify LLVM calling conventions and map them
 * to Lynx’s internal calling convention types.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_TAIL_CALLING_CONV_SPEC_HPP
#define LYNX_TAIL_CALLING_CONV_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class TailSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::Tail;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::Tail; }

            ~TailSpec() override = default;
        };
        
}

#endif 
