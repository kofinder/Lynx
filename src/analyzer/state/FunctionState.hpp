#ifndef LYNX_FUNCTION_STATE_HPP
#define LYNX_FUNCTION_STATE_HPP

#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    class FunctionState : public SemanticState {

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    
    };
}

#endif 
