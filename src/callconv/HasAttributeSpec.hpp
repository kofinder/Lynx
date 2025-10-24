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
                LOG_INFO("Invoked");
                return func->hasFnAttribute("cold");
            }

            CallingConventionType getCallingConvention() const override { return ccType; }
    };
}


#endif 
