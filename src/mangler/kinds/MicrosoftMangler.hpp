/**
 * @file MicrosoftMangler.hpp
 * @brief Implements a Microsoft Visual C++-style name mangling strategy for Lynx.
 *
 * The `MicrosoftMangler` class implements the `IMangleStrategy` interface and provides
 * methods to mangle:
 * - Free functions
 * - Member functions
 * - Constructors
 * - Class names
 *
 * Currently, all methods return `nullptr` as placeholders and need proper implementation
 * following the Microsoft C++ ABI mangling conventions.
 *
 * Example Usage:
 * @code
 * LynxMangler::MicrosoftMangler mangler;
 * std::string mangled = mangler.mangleFunction("foo", params);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_MICROSOFT_MANGLER_HPP
#define LYNX_MICROSOFT_MANGLER_HPP

#include <mangler/IMangleStrategy.hpp>
#include <mangler/MangleParamter.hpp>
#include <mangler/MangleParamter.hpp>

namespace LynxMangler {

    class MicrosoftMangler : public IMangleStrategy {

        public:

            std::string mangleFunction(const std::string& funcName, const ParameterTypes& paramTypes) override {
                return ""; 
            }

            std::string mangleMemberFunction(const std::string& className, const std::string& funcName, const ParameterTypes& paramTypes) override {
                return ""; 
            }
        
            std::string mangleConstructor(const std::string& className, const ParameterTypes& paramTypes) override {
                return ""; 
            }
        
            std::string mangleClass(const std::string& className) override {
                return ""; 
            }
    };
}

#endif
