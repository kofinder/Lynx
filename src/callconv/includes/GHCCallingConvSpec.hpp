/**
 * @file GHCCallingConvSpec.hpp
 * @brief Defines a specification for detecting the LLVM GHC calling convention.
 *
 * @responsibilities
 * - Detects LLVM functions using the `GHC` (Glasgow Haskell Compiler) calling convention.
 * - Maps detected functions to the internal `CallingConventionType::GHC` type.
 * - Enables specialized handling of Haskell-style runtime and tail-call heavy functions.
 *
 * @namespace LynxCallConv
 * Contains specifications that identify and classify LLVM calling conventions
 * used within the Lynx compiler. Each specification encapsulates a detection rule
 * for a specific calling convention type.
 *
 * @note
 * The `GHC` calling convention is primarily used by the Glasgow Haskell Compiler (GHC)
 * to support efficient functional-style execution models with optimized tail calls.
 *
 * @autor: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_GHC_CALLING_SPEC_HPP
#define LYNX_CALLING_CONV_GHC_CALLING_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class GHCCallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::GHC;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override { return CallingConventionType::GHC; }

            ~GHCCallingConvSpec() override = default;

    };
        
}


#endif 
