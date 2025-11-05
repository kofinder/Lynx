/**
 * @file PreserveMostSpec.hpp
 * @brief Defines a specification for detecting functions using the LLVM PreserveMost calling convention.
 *
 * @responsibilities
 * - Detects LLVM functions that utilize the `PreserveMost` calling convention.
 * - Provides mapping to the internal `CallingConventionType::PreserveMost`.
 * - Enables Lynx’s compiler backend to identify functions optimized for minimal register preservation.
 *
 * @namespace LynxCallConv
 * Contains specifications responsible for detecting and classifying LLVM calling conventions
 * within the Lynx compiler infrastructure.
 *
 * @note
 * The `PreserveMost` calling convention is designed to minimize register preservation
 * while maintaining ABI compatibility, offering a performance balance between safety and efficiency.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CALLING_CONV_PRESERVE_MOST_SPEC_HPP
#define LYNX_CALLING_CONV_PRESERVE_MOST_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    using namespace LynxConstants;

    class PreserveMostSpec : public CallingConventionSpecification {
        
        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::PreserveMost;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::PreserveMost; }

            ~PreserveMostSpec() override = default;

    };
}

#endif 
