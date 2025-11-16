#ifndef LYNX_TYPE_STRATEGY_CONTEXT
#define LYNX_TYPE_STRATEGY_CONTEXT

#include <llvm/IR/Value.h>
#include <type_traits>
#include <vector>
#include <context/AstContext.hpp>

namespace LynxTypes {

    using LynxContext::AstContext;


    // // =========================
    // // Concepts for Numeric Types
    // // =========================
    // template<typename T>
    // concept Numeric = std::is_arithmetic_v<T>; 

    // template<typename T>
    // concept Integer = std::is_integral_v<T>;

    // template<typename T>
    // concept Floating = std::is_floating_point_v<T>;


    // Integer concept (excludes bool)
    template<typename T>
    concept IntStrategyType = std::is_integral_v<T> && !std::is_same_v<T, bool>;

    // Floating point concept
    template<typename T>
    concept FloatStrategyType = std::is_floating_point_v<T>;

    struct StrategyContext {
        const AstContext& ctx;
        llvm::Value* instance;
        const std::vector<llvm::Value*>& args;
    };
}

#endif
