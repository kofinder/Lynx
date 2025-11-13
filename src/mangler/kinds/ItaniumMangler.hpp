/**
 * @file ItaniumMangler.hpp
 * @brief Implements the Itanium C++ ABI-style name mangling strategy for Lynx.
 *
 * The `ItaniumMangler` class implements the `IMangleStrategy` interface and provides
 * methods to mangle:
 * - Free functions
 * - Member functions
 * - Constructors
 * - Class names
 *
 * Mangling follows the Itanium C++ ABI conventions, using prefixes, nested names,
 * and parameter type encoding as defined in ItaniumTypeEncoding and ItaniumManglerPrefix.
 *
 * Example Usage:
 * @code
 * LynxMangler::ItaniumMangler mangler;
 * std::string mangled = mangler.mangleFunction("foo", params);
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ITANIUM_MANGLER_HPP
#define LYNX_ITANIUM_MANGLER_HPP

#include <logger/Logger.hpp>
#include <mangler/IMangleStrategy.hpp>
#include <mangler/MangleParamter.hpp>
#include <mangler/utils/TypeEncoderUtils.hpp>
#include <mangler/constants/ItaniumManglerPrefix.hpp>
#include <mangler/constants/ItaniumTypeEncoding.hpp>


namespace LynxMangler {

    using namespace LynxLogger;

    class ItaniumMangler : public IMangleStrategy {

        public:
            
            std::string mangleFunction(const std::string& funcName, const ParameterTypes& paramTypes) override {
                std::string mangled = MANGLE_PREFIX;
                mangled += encodeNameComponent(funcName);
                mangled += encodeParameters(paramTypes);
                return mangled;
            }

            std::string mangleMemberFunction(const std::string& className, const std::string& funcName, const ParameterTypes& paramTypes) override {
                std::string mangled = MANGLE_PREFIX;
                mangled += PREFIX_NESTED_NAME;
                mangled += encodeNameComponent(className);
                mangled += encodeNameComponent(funcName);
                mangled += SUFFIX_END_NESTED_NAME;
                mangled += encodeParameters(paramTypes);
                return mangled;            
            }

            std::string mangleConstructor(const std::string& className, const ParameterTypes& paramTypes) override {
                std::string mangled = MANGLE_PREFIX;
                mangled += PREFIX_NESTED_NAME;
                mangled += encodeNameComponent(className);
                mangled += SUFFIX_CONSTRUCTOR_PRIMARY;
                mangled += SUFFIX_END_NESTED_NAME;
                mangled += encodeParameters(paramTypes);
                return mangled;        
            }

            std::string mangleClass(const std::string& className) override {
                std::string mangled = MANGLE_PREFIX;
                mangled += encodeNameComponent(className);
                return mangled;
            }
    };
}

#endif
