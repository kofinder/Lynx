#ifndef LYNX_SEMANTIC_STATE_FACTORY_HPP
#define LYNX_SEMANTIC_STATE_FACTORY_HPP

#include <vector>
#include <memory>

#include "SemanticState.hpp"
#include "state/ClassState.hpp"
#include "state/InterfaceState.hpp"
#include "state/EnumState.hpp"
#include "state/FunctionState.hpp"
#include "state/VariableState.hpp"
#include "state/MixinState.hpp"


namespace LynxAnalyzer {

    class SemanticStateFactory {

        public:

            static std::vector<std::unique_ptr<SemanticState>> createAllStates() {
                std::vector<std::unique_ptr<SemanticState>> states;
                states.push_back(std::make_unique<ClassState>());
                states.push_back(std::make_unique<InterfaceState>());
                states.push_back(std::make_unique<MixinState>());
                states.push_back(std::make_unique<EnumState>());
                states.push_back(std::make_unique<FunctionState>());
                states.push_back(std::make_unique<VariableState>());
                return states;
            }
            
        };
        
}

#endif
