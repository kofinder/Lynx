#ifndef LYNX_ENUM_STATE_HPP
#define LYNX_ENUM_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    class EnumState : public SemanticState {

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
}

#endif 
