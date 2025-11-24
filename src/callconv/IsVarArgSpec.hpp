/**
 * @file IsVarArgSpec.hpp
 * @brief Defines a specification that checks whether a function uses variable arguments.
 *
 * The `IsVarArgSpec` class implements the `CallingConventionSpecification` interface
 * and determines whether an LLVM function is declared as variadic (i.e., it accepts
 * a variable number of arguments, such as `printf` or `fprintf`).
 *
 * This specification is often used in combination with other calling convention rules
 * (e.g., `AndSpecification`) to refine inference logic.
 * @see CallingConventionSpecification
 * @see AndSpecification
 * @see CallingConventionInferer
 * 
 * @namespace LynxCallConv
 * Contains rule-based specifications for detecting calling conventions in LLVM IR.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_IS_VAR_ARG_SPEC_HPP
#define LYNX_IS_VAR_ARG_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class IsVarArgSpec : public CallingConventionSpecification {

        private:

            CallingConventionType ccType;

        public:

            explicit IsVarArgSpec(CallingConventionType type) : ccType(type) {}


            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->isVarArg();
                LOG_INFO("Is Var Argument is {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override { return ccType; }
    };
}


#endif 
