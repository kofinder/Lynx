#ifndef LYNX_CALLING_CONV_HIPE_SPEC_HPP
#define LYNX_CALLING_CONV_HIPE_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

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
