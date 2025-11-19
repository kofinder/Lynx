#ifndef LYNX_RUNTIME_ARRAY_FUNCTIONS_HPP
#define LYNX_RUNTIME_ARRAY_FUNCTIONS_HPP

#include "includes/AtFunction.hpp"
#include "RuntimeFunctionRegistry.hpp"

namespace LynxRuntime {

    void registerArrayFunctions(RuntimeFunctionRegistry& registry) {
        registry.registerFunction("Arrays.at", std::make_shared<AtFunction>());
    }
    
}

#endif 
