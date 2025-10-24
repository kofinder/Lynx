#ifndef LYNX_MIXIN_STATE_HPP
#define LYNX_MIXIN_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"
#include <types/userdefined/MixinType.hpp>

namespace LynxAnalyzer {
    
    using namespace LynxTypes;

    class MixinState : public SemanticState {
        
        private:

            bool detectCycle(MixinType* mixin, std::unordered_map<std::string,int>& state, std::vector<std::string>& stack, std::string& errMsg) const;

            bool validateFlattenedMixin(MixinType* mixin, std::string& errMsg) const;

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
        
}

#endif 
