#ifndef LYNX_CALLING_CONV_PRESERVE_ALL_SPEC_HPP
#define LYNX_CALLING_CONV_PRESERVE_ALL_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class PreserveAllSpec : public CallingConventionSpecification {
        
        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::PreserveAll;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::PreserveAll; }

            ~PreserveAllSpec() override = default;

    };
        
        
}

#endif 
