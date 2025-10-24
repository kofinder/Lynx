#ifndef LYNX_CALLING_CONV_GHC_CALLING_SPEC_HPP
#define LYNX_CALLING_CONV_GHC_CALLING_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

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
