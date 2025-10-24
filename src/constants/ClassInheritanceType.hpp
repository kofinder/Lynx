#ifndef LYNX_CLASS_INHERITANCE_TYPE_HPP
#define LYNX_CLASS_INHERITANCE_TYPE_HPP

#include <vector>
#include <optional>
#include <string>

namespace LynxConstants {

    struct ClassInheritanceType {

        std::optional<std::string> baseClass;           // from "extends"

        std::vector<std::string> mixins;                // from "with"

        std::vector<std::string> interfaces;            // from "implements"

    };
    
}

#endif
