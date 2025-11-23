#ifndef LYNX_TYPE_STRATEGY_CONTEXT
#define LYNX_TYPE_STRATEGY_CONTEXT

#include <llvm/IR/Value.h>
#include <type_traits>
#include <vector>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;

    // Integer concept (excludes bool)
    template<typename T>
    concept IntStrategyType = std::is_integral_v<T> && !std::is_same_v<T, bool>;

    // Floating point concept
    template<typename T>
    concept FloatStrategyType = std::is_floating_point_v<T>;

    struct StrategyContext {
        const AstContext& ctx;
        llvm::Value* instance;
        llvm::Value* instancePtr;
        std::vector<llvm::Value*> args;
    };
}

#endif
