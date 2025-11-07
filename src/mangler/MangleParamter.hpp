/**
 * @file MangleParameter.hpp
 * @brief Defines the structures and types for name mangling parameters in Lynx.
 *
 * This header provides the `MangleParameter` struct and the `ParameterTypes` variant
 * used to represent the necessary information for mangling functions, member functions,
 * constructors, and classes. It supports overloading, class scoping, and optional namespaces.
 *
 * **Key Components:**
 * - `ParameterTypes`: A variant representing a list of argument types as either:
 *      - Strings (type names)
 *      - `BaseType*` (Lynx type system objects)
 *      - `llvm::Type*` (LLVM types)
 *
 * - `MangleParameter`: Stores the kind of symbol (function, member function, constructor, class),
 *   optional namespace, class name, method name, and the argument types.
 *
 * **Factory Methods:**
 * - `makeFunction` : Creates a `MangleParameter` for a standalone function.
 * - `makeMemberFunction` : Creates a `MangleParameter` for a class member function.
 * - `makeConstructor` : Creates a `MangleParameter` for a class constructor.
 * - `makeClass` : Creates a `MangleParameter` for a class itself.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_MANGLE_PARAMETER_HPP
#define LYNX_MANGLE_PARAMETER_HPP

#include <string>
#include <optional>
#include <constants/ManglerType.hpp>
#include <types/interfaces/BaseType.hpp>

namespace LynxMangler {

    using namespace LynxTypes;
    using namespace LynxConstants;

    using ParameterTypes = std::variant<
        std::reference_wrapper<const std::vector<std::string>>,
        std::reference_wrapper<const std::vector<BaseType*>>,
        std::reference_wrapper<const std::vector<llvm::Type*>>
    >;

    struct MangleParameter {

        ManglerKind kind;

        std::optional<std::string> nameSpace;

        std::optional<std::string> clazzName;

        std::optional<std::string> methodName;

        ParameterTypes args; 


        static MangleParameter makeFunction(const std::string& funcName, ParameterTypes args, std::optional<std::string> ns = std::nullopt) {
            return { ManglerKind::FUNCTION, ns, std::nullopt, funcName, args };
        }

        static MangleParameter makeMemberFunction(const std::string& clazzName, const std::string& funcName, ParameterTypes args, std::optional<std::string> ns = std::nullopt) {
            return { ManglerKind::MEMBER_FUNCTION, ns, clazzName, funcName, args };
        }

        static MangleParameter makeConstructor(const std::string& clazzName, ParameterTypes args, std::optional<std::string> ns = std::nullopt) {
            return { ManglerKind::CONSTRUCTOR, ns, clazzName, std::nullopt, args };
        }

        static MangleParameter makeClass(const std::string& clazzName, std::optional<std::string> ns = std::nullopt ) {
            static const std::vector<std::string> emptyArgs{};
            return { ManglerKind::CLASS, ns, clazzName, std::nullopt, std::cref(emptyArgs) };
        }
    };

}

#endif