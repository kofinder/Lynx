#ifndef LYNX_MICROSOFT_MANGLER_HPP
#define LYNX_MICROSOFT_MANGLER_HPP

#include <mangler/IMangleStrategy.hpp>
#include <mangler/MangleParamter.hpp>
#include <mangler/MangleParamter.hpp>

namespace LynxMangler {

    class MicrosoftMangler : public IMangleStrategy {

        public:

            std::string mangleFunction(const std::string& funcName, const ParameterTypes& paramTypes) override {
                return nullptr;
            }

            std::string mangleMemberFunction(const std::string& className, const std::string& funcName, const ParameterTypes& paramTypes) override {
                return nullptr;
            }
        
            std::string mangleConstructor(const std::string& className, const ParameterTypes& paramTypes) override {
                return nullptr;
            }
        
            std::string mangleClass(const std::string& className) override {
                return nullptr;
            }
    };
}

#endif
