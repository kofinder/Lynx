/**
 * @file CustomMangler.hpp
 * @brief Implements a custom mangling strategy for the Lynx compiler.
 *
 * The `CustomMangler` class provides a user-defined name mangling strategy
 * that conforms to the `IMangleStrategy` interface. It allows mangling of:
 * - Free functions
 * - Member functions
 * - Constructors
 * - Class names
 *
 * **Note:** Currently, all mangling methods return `nullptr`. These should be
 * implemented to generate valid mangled names according to the Lynx custom mangling rules.
 *
 * Example Usage:
 * @code
 * LynxMangler::CustomMangler mangler;
 * auto mangledName = mangler.mangleFunction("foo", params);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_CUSTOM_MANGLER_HPP
#define LYNX_CUSTOM_MANGLER_HPP

#include <mangler/IMangleStrategy.hpp>
#include <mangler/MangleParamter.hpp>
#include <mangler/MangleParamter.hpp>

namespace LynxMangler {
    
    class CustomMangler : public IMangleStrategy {

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
