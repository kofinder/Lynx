#ifndef LYNX_IMPORT_TYPE_HPP
#define LYNX_IMPORT_TYPE_HPP

#include <array>

namespace LynxConstants {
    constexpr std::array<const char*, 3> runtime_import_files = {
        "filesystem",
        "collections",
        "logger"
    };

    enum ImportType {
        Named,
        Default,
        Namespace,
        SideEffect
    };    
}

#endif 
