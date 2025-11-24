/**
 * @file HasAttributeSpec.hpp
 * @brief Defines a specification that checks for the presence of a specific LLVM function attribute.
 *
 * This class implements the `CallingConventionSpecification` interface and determines
 * whether a given LLVM function declares a specific attribute, such as `"cold"` or `"fast"`.
 *
 * It is used by the `CallingConventionInferer` to help identify the calling convention type
 * based on function-level attributes embedded within the LLVM IR.
 *
 * @see CallingConventionSpecification
 * @see CallingConventionInferer
 * @see AndSpecification
 * 
 * @namespace LynxCallConv
 * Contains rule-based specifications for detecting calling conventions in LLVM IR.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
 */


#ifndef LYNX_HAS_ATTRIBUTE_SPEC_HPP
#define LYNX_HAS_ATTRIBUTE_SPEC_HPP

#include <string>
#include "CallingConventionSpecification.hpp"

using namespace LynxConstants;

namespace LynxCallConv {

    class HasAttributeSpec : public CallingConventionSpecification {

        private:

            std::string attrName;

            CallingConventionType ccType;


        public:

            explicit HasAttributeSpec(const std::string& name, CallingConventionType type) : attrName(name), ccType(type) {}

            bool isSatisfiedBy(llvm::Function* func) const override {
                return func->hasFnAttribute("cold");
            }

            CallingConventionType getCallingConvention() const override { return ccType; }
    };
}


#endif 
