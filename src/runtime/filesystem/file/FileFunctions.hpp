#ifndef LYNX_RUNTIME_FILE_FUNCTIONS_HPP
#define LYNX_RUNTIME_FILE_FUNCTIONS_HPP

#include "includes/FSOpenFunction.hpp"
#include "includes/FSWriteFunction.hpp"
#include "includes/FSReadFunction.hpp"
#include "includes/FSExistsFunction.hpp"
#include "includes/FSCloseFunction.hpp"
#include "RuntimeFunctionRegistry.hpp"

namespace LynxRuntime {

    void registerFileFunctions(RuntimeFunctionRegistry& registry) {
        registry.registerFunction("fs::open", std::make_shared<FSOpenFunction>());
        registry.registerFunction("fs::write", std::make_shared<FSWriteFunction>());
        registry.registerFunction("fs::read", std::make_shared<FSReadFunction>());
        registry.registerFunction("fs::exists", std::make_shared<FSExistsFunction>());
        registry.registerFunction("fs::close", std::make_shared<FSCloseFunction>());
    }
    
}

#endif 
