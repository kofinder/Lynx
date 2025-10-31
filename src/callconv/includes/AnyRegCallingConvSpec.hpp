#ifndef LYNX_CALLING_CONV_ANY_REG_SPEC_HPP
#define LYNX_CALLING_CONV_ANY_REG_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxConstants;

namespace LynxCallConv {

    class AnyRegCallingConvSpec : public CallingConventionSpecification {

        public:

            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == llvm::CallingConv::AnyReg;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }

            CallingConventionType getCallingConvention() const override { return CallingConventionType::AnyReg; }

            ~AnyRegCallingConvSpec() override = default;

    };      
        
}


#endif 
