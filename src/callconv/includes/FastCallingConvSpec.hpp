#ifndef LYNX_FAST_CALLING_CONV_SPEC_HPP
#define LYNX_FAST_CALLING_CONV_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class FastCallingConvSpec : public CallingConventionSpecification {

        public:
            bool isSatisfiedBy(llvm::Function* func) const override {
                // bool isEqual = func->hasFnAttribute("fast") && !func->isVarArg();
                // LOG_INFO("isSatisfiedBy {}", isEqual);

                bool isEqual = func->getCallingConv() == llvm::CallingConv::Fast;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override { return CallingConventionType::Fast; }

            ~FastCallingConvSpec() override = default;

    };
}


#endif 
