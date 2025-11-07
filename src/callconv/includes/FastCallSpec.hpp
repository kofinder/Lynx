/**
 * @file FastCallSpec.hpp
 * @brief Defines a specification for detecting the LLVM FastCall calling convention.
 *
 * @responsibilities
 * - Detects LLVM functions that use the `FastCall` calling convention.
 * - Associates detected functions with `CallingConventionType::FastCall`.
 * - Provides convention-specific validation for function-level analysis.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify and classify calling conventions
 * used by LLVM functions in the Lynx compilation system. Each specification
 * provides a mechanism to check if a function matches a specific convention.
 *
 * @note
 * The `FastCall` convention is commonly used on Windows platforms and passes
 * arguments through registers to improve performance, reducing stack usage.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_FAST_CALL_SPEC_HPP
#define LYNX_CALLING_CONV_FAST_CALL_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class FastCallSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::Fast;
                LOG_INFO("isSatisfiedBy {}", isEqual);;
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::FastCall; }

            ~FastCallSpec() override = default;

        };
        
}


#endif 
