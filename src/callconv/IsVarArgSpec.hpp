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
