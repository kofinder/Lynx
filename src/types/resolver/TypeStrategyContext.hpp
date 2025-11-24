/**
 * @file StrategyContext.hpp
 * @brief Defines the StrategyContext structure and numeric type concepts for LLVM IR code generation.
 *
 * This header provides a unified context for generating LLVM IR instructions 
 * for various numeric strategies. It encapsulates the current AST context, 
 * the primary value, its pointer, and a list of argument values.
 *
 * Key components:
 *  - `StrategyContext`: stores references to the AST context, the instance value,
 *    its pointer, and the argument list for instruction generation.
 *  - `IntStrategyType` and `FloatStrategyType`: concepts to constrain template
 *    strategies to integer (excluding bool) or floating-point types.
 *
 * Features:
 *  - Type-safe context for numeric operations and intrinsics.
 *  - Facilitates template-based strategy implementations.
 *  - Compatible with LLVM IRBuilder usage for code generation.
 *
 * Benefits:
 *  - Centralizes state needed for numeric instruction strategies.
 *  - Ensures type correctness in template specializations.
 *  - Improves maintainability and readability of backend code.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:   Jan 2, 2022
*/

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
        const AstContext& ctx; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        llvm::Value* instance;
        llvm::Value* instancePtr;
        std::vector<llvm::Value*> args;
    };
}

#endif
