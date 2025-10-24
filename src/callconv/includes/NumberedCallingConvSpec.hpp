#ifndef LYNX_CALLING_CONV_NUMBER_CALL_SPEC_HPP
#define LYNX_CALLING_CONV_NUMBER_CALL_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class NumberedCallingConvSpec : public CallingConventionSpecification {

        unsigned ccNum;

        public:
        
            NumberedCallingConvSpec() : ccNum(0) {}

            explicit NumberedCallingConvSpec(unsigned num) : ccNum(num) {}
        
            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = func->getCallingConv() == ccNum;
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return false;
            }
        
            CallingConventionType getCallingConvention() const override { return CallingConventionType::Numbered; }

            ~NumberedCallingConvSpec() override = default;

        };
    
        
        
}


#endif 
