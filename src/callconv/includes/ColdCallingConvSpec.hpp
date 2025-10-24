#ifndef LYNX_CALLING_CONV_COLD_SPEC_HPP
#define LYNX_CALLING_CONV_COLD_SPEC_HPP


#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class ColdCallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->hasFnAttribute("cold");
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
            
            CallingConventionType getCallingConvention() const override {  return CallingConventionType::Cold; }

            ~ColdCallingConvSpec() override = default;

    };
}


#endif 
