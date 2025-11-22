/**
 * @file DFSUtils.hpp
 * @brief Provides utilities for depth-first search and topological sorting of mixins.
 *
 * This header defines functions and enums to perform DFS-based traversal of 
 * `MixinType` hierarchies, detect cycles, and generate a topological ordering
 * of mixins. It is primarily used during type resolution and mixin composition
 * in the Lynx compiler.
 *
 * **Key Components:**
 * - `VisitState` enum: Tracks DFS visitation status.
 * - `topoDFS`: Recursively visits mixins, detects cycles, and produces a topological order.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_DFS_UTILS_HPP
#define LYNX_TYPE_DFS_UTILS_HPP

#include <unordered_map>
#include <types/userdefined/MixinType.hpp>

namespace LynxTypes::DFSUtils {


    enum class VisitState : std::uint8_t { UNVISITED, VISITING, VISITED };

    // NOLINTNEXTLINE(misc-no-recursion)
    inline bool topoDFS(
        const MixinType* mixin,
        std::unordered_map<const MixinType*, VisitState>& state,
        std::vector<const MixinType*>& order
    ) {
        auto it = state.find(mixin);
        if (it != state.end()) {
            if (it->second == VisitState::VISITING) {
                throw std::runtime_error("Cycle detected in mixins: " + mixin->originalName());
            }
            if (it->second == VisitState::VISITED) return false;
        }

        state[mixin] = VisitState::VISITING;

        for (const auto& parent : mixin->getParents()) { // assumes MixinType has getMixins()
            topoDFS(parent, state, order);
        }

        state[mixin] = VisitState::VISITED;

        order.push_back(mixin); // add after children

        return true;
    }
}

#endif
