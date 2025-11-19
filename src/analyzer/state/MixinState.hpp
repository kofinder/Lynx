/**
 * @file MixinState.hpp
 * @brief Declares the MixinState class, a semantic state for handling mixin nodes in the AST.
 * 
 * MixinState is responsible for analyzing mixin declarations, inheritance, and flattening
 * within the Lynx language. It works with SemanticAnalyzer to ensure correctness of mixin usage,
 * detect inheritance cycles, and validate flattened mixin structures.
 * 
 * **Key Responsibilities:**
 * - Detect cycles in mixin inheritance hierarchies.
 * - Validate flattened mixin structures to ensure consistency.
 * - Integrate mixin-related symbols into the semantic analysis workflow.
 * 
 * **Used By:**
 * - SemanticAnalyzer for parallel AST semantic checks.
 * - SemanticStateFactory for automatic state registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ANALYZER_MIXIN_STATE_HPP
#define LYNX_ANALYZER_MIXIN_STATE_HPP

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
