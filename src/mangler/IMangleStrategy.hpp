#ifndef LYNX_IMANGLE_STRATEGY_HPP
#define LYNX_IMANGLE_STRATEGY_HPP

#include <iostream>
#include <string>
#include <vector>
#include "MangleParamter.hpp"

namespace LynxMangler {

    class IMangleStrategy {

        public:

            virtual ~IMangleStrategy() = default;

            virtual std::string mangleFunction(const std::string& name, const ParameterTypes& paramTypes) = 0;

            virtual std::string mangleMemberFunction(const std::string& className, const std::string& funcName, const ParameterTypes& paramTypes) = 0;
           
            virtual std::string mangleConstructor(const std::string& className, const ParameterTypes& paramTypes) = 0;
         
            virtual std::string mangleClass(const std::string& className) = 0;
    };

}

#endif
