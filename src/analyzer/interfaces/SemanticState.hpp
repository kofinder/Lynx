#ifndef LYNX_SEMANTIC_STATE_HPP
#define LYNX_SEMANTIC_STATE_HPP

#include <string>
#include <vector>
#include <optional>
#include <ast/Node.hpp>

namespace LynxAnalyzer {

    using namespace LynxAst;

    class SemanticAnalyzer;

    class SemanticState {

        public:

            virtual void handleNode(SemanticAnalyzer& analyzer, Node* node) = 0;
            
            virtual ~SemanticState() {}
    };

}

#endif
