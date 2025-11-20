#include "state/MixinState.hpp"
#include "interfaces/SemanticAnalyzer.hpp"

namespace LynxAnalyzer {


    bool MixinState::detectCycle(MixinType* /*mixinType*/, std::unordered_map<std::string,int>& /*state*/, std::vector<std::string>& /*stack*/, std::string& /*errMsg*/) const {
        return false;
    }

    bool MixinState::validateFlattenedMixin(MixinType*, std::string&) const {
        return false;
    }

    void MixinState::handleNode(SemanticAnalyzer& /*analyzer*/, Node* /*node*/) {
    }

}