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