/**
 * @file IMangleStrategy.hpp
 * @brief Defines the interface for name mangling strategies in the Lynx compiler.
 *
 * The `IMangleStrategy` interface provides an abstraction for implementing
 * different name mangling schemes for functions, member functions, constructors,
 * and classes. Name mangling encodes type information into identifiers to
 * support function overloading, class scoping, and linkage.
 *
 * **Key Methods:**
 * - `mangleFunction` : Mangles a standalone function name with parameter types.
 * - `mangleMemberFunction` : Mangles a class member function including class name and parameter types.
 * - `mangleConstructor` : Mangles a class constructor with parameter types.
 * - `mangleClass` : Mangles a class name for linkage purposes.
 *
 * This interface enables flexibility in defining multiple mangling schemes
 * within the compiler or code generation pipeline.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/


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
