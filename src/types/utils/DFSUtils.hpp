#ifndef LYNX_TYPE_DFS_UTILS_HPP
#define LYNX_TYPE_DFS_UTILS_HPP

#include <unordered_map>
#include <types/userdefined/MixinType.hpp>

namespace LynxTypes::DFSUtils {


    enum class VisitState { UNVISITED, VISITING, VISITED };

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
