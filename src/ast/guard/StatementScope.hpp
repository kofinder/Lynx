#ifndef LYNX_STATEMENT_SCOPE_GUARD_HPP
#define LYNX_STATEMENT_SCOPE_GUARD_HPP

#include <context/GlobalSymbolContext.hpp>

namespace LynxAst::guard {

    using namespace LynxContext;

    struct StatementScope {

        GlobalSymbolContext& symbolContext;

        StatementScope(
            GlobalSymbolContext& ctx, 
            Node* stmtNode
        ) : symbolContext(ctx) {
            symbolContext.addStatement(stmtNode);
        }

        ~StatementScope() { symbolContext.removeStatement(); }
    };
}

#endif


