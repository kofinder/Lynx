#ifndef LYNX_CALLING_CONV_SWIFT_TAIL_SPEC_HPP
#define LYNX_CALLING_CONV_SWIFT_TAIL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class SwiftTailSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::SwiftTail;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::SwiftTail; }

            ~SwiftTailSpec() override = default;
        };
        
}


#endif 
