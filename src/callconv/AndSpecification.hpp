#ifndef LYNX_CALLING_CONV_AND_SPECIFICATION_HPP
#define LYNX_CALLING_CONV_AND_SPECIFICATION_HPP

#include "CallingConventionSpecification.hpp"


namespace LynxCallConv {

    class AndSpecification : public CallingConventionSpecification {

        std::unique_ptr<CallingConventionSpecification> left;
        std::unique_ptr<CallingConventionSpecification> right;
        CallingConventionType convType;
    
        public:
        
            AndSpecification(
                std::unique_ptr<CallingConventionSpecification> l,
                std::unique_ptr<CallingConventionSpecification> r,
                CallingConventionType type
            ): left(std::move(l)), right(std::move(r)), convType(type) {}


            bool isSatisfiedBy(llvm::Function* func) const override {
                bool isEqual = left->isSatisfiedBy(func) && right->isSatisfiedBy(func);
                LOG_INFO("isSatisfiedBy {}", isEqual);
                return isEqual;
            }
        
            CallingConventionType getCallingConvention() const override { return convType; }
    };
}

#endif 
