#ifndef LYNX_VARIABLE_STATE_HPP
#define LYNX_VARIABLE_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    class VariableState : public SemanticState {

        public:
        
            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;

    };
        
}

#endif 
