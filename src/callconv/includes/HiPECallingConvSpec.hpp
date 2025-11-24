/**
 * @file HiPECallingConvSpec.hpp
 * @brief Defines a specification for detecting the LLVM HiPE (High-Performance Erlang) calling convention.
 *
 * @responsibilities
 * - Identifies LLVM functions that use the `HiPE` calling convention.
 * - Associates the detected convention with `CallingConventionType::HiPE` for internal use.
 * - Supports recognition of Erlang’s native code interface conventions in LLVM IR.
 *
 * @namespace LynxCallConv
 * Contains specifications responsible for detecting and classifying LLVM calling conventions
 * within the Lynx compiler. Each specification corresponds to a specific LLVM calling convention type.
 *
 * @note
 * The `HiPE` calling convention is used by the High-Performance Erlang (HiPE) compiler backend
 * to support efficient function calls in Erlang’s native code execution environment.
 *
 * @autor: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/



#ifndef LYNX_CALLING_CONV_HIPE_SPEC_HPP
#define LYNX_CALLING_CONV_HIPE_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class HiPECallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::HiPE;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::HiPE; }

            ~HiPECallingConvSpec() override = default;

    };
        
}


#endif 
