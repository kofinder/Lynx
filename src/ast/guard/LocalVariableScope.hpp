#ifndef LYNX_LOCAL_VARIABLE_SCOPE_GUARD_HPP
#define LYNX_LOCAL_VARIABLE_SCOPE_GUARD_HPP

#include <context/GlobalSymbolContext.hpp>

namespace LynxAst::guard {

    using namespace LynxContext;

    struct LocalVariableScope {

        GlobalSymbolContext& symbolContext;

        LocalVariableScope(
            GlobalSymbolContext& ctx, 
            const std::string& varName, 
            llvm::Value* value
        ) : symbolContext(ctx) {
            symbolContext.registerLocalVariable(varName, value);
        }

        ~LocalVariableScope() { symbolContext.clearLocalVariables(); }
    };
}

#endif


