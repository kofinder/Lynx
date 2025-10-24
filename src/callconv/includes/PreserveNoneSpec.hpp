#ifndef LYNX_CALLING_CONV_PRESERVE_NONE_SPEC_HPP
#define LYNX_CALLING_CONV_PRESERVE_NONE_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class PreserveNoneSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                //return func->getCallingConv() == llvm::CallingConv::PreserveNone;
                LOG_INFO("isSatisfiedBy {}", false);
                return false;
            }

            CallingConventionType getCallingConvention() const override {
                return CallingConventionType::PreserveNone;
            }

            ~PreserveNoneSpec() override = default;

        };
         
        
}

#endif 
