#ifndef LYNX_CALLING_CONV_CXX_FAST_TL_SPEC_HPP
#define LYNX_CALLING_CONV_CXX_FAST_TL_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class CXXFastTLSSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::CXX_FAST_TLS;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::CXXFastTLS; }

            ~CXXFastTLSSpec() override = default;

        };
        
}

#endif 
