#ifndef LYNX_LIB_RUNTIME_ARRAY_FUNCTIONS_HPP
#define LYNX_LIB_RUNTIME_ARRAY_FUNCTIONS_HPP

#include "includes/AtFunction.hpp"
#include "RuntimeFunctionRegistry.hpp"

namespace LynxLibRuntime {

    void registerArrayFunctions(RuntimeFunctionRegistry& registry) {
        registry.registerFunction("Arrays.at", std::make_shared<AtFunction>());
    }
    
}

#endif 
