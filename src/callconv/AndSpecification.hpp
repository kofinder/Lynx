/**
 * @file AndSpecification.hpp
 * @brief Defines the logical AND specification for LLVM calling convention evaluation.
 * 
 * The `AndSpecification` class implements a **composite specification pattern**
 * that combines two `CallingConventionSpecification` objects. It returns `true`
 * only when **both specifications** are satisfied for a given LLVM function.
 * 
 * This class is part of the calling convention analysis system used by the Lynx compiler
 * to enforce and infer function calling conventions based on multiple combined criteria.
 * 
 * @see CallingConventionSpecification
 * @see CallingConventionType
 * 
 * @namespace LynxCallConv
 * Provides components and logic for LLVM calling convention analysis and inference.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


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
