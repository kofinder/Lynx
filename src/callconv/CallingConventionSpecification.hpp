/**
 * @file CallingConventionSpecification.hpp
 * @brief Defines the abstract interface for calling convention specifications.
 * 
 * This file introduces the `CallingConventionSpecification` abstract base class,
 * which represents a single rule or condition used to determine whether an LLVM
 * function matches a particular calling convention type.
 * 
 * Each subclass defines its own logic in `isSatisfiedBy()` to check attributes,
 * function properties, or patterns that correspond to a specific calling convention.
 * 
 * This class is used by `CallingConventionInferer` to iterate through multiple
 * specifications and infer the most appropriate calling convention.
 * 
 * @see CallingConventionInferer
 * @see AndSpecification
 * @see HasAttributeSpec
 * @see IsVarArgSpec
 * 
 * @namespace LynxCallConv
 * Provides specification-based mechanisms for identifying LLVM calling conventions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
 */

#ifndef LYNX_CALLING_CONV_SPECIFICATION_HPP
#define LYNX_CALLING_CONV_SPECIFICATION_HPP

#include <llvm/IR/Function.h>
#include <logger/Logger.hpp>
#include <constants/CallingConventionType.hpp>

using namespace LynxConstants;
using namespace LynxLogger;

namespace LynxCallConv {

    class CallingConventionSpecification {

        public:

            /**
             * @brief Checks if the given function satisfies this calling convention specification.
             * 
             * @param func Pointer to the LLVM function to check.
             * @return true if the function satisfies the specification, false otherwise.
            */
            virtual bool isSatisfiedBy(llvm::Function* func) const = 0;

            /**
             * @brief Returns the calling convention type associated with this specification.
             * 
             * @return CallingConventionType enum value representing the calling convention.
            */
            virtual CallingConventionType getCallingConvention() const = 0;
            
            /**
             * @brief Virtual destructor for proper cleanup in derived classes.
            */
            virtual ~CallingConventionSpecification() = default;

        };
}

#endif 
