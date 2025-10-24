#ifndef LYNX_FUNCTION_SCOPE_GUARD_HPP
#define LYNX_FUNCTION_SCOPE_GUARD_HPP

#include <context/GlobalSymbolContext.hpp>

namespace LynxAst::guard {

    using namespace LynxContext;

    struct FunctionScope {

        GlobalSymbolContext& symbolContext;

        FunctionScope(
            GlobalSymbolContext& ctx, 
            FunctionNode* fnNode
        ) : symbolContext(ctx) {
            symbolContext.addFunction(fnNode);
        }

        ~FunctionScope() { 
            symbolContext.removeFunction(); 
            symbolContext.removeMethodCall();
            symbolContext.clearLocalVariables();
        }
    };
}

#endif
