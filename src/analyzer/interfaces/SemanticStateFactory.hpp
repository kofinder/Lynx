/**
 * @file SemanticStateFactory.hpp
 * @brief Declares the SemanticStateFactory class, responsible for creating all semantic analysis states.
 * 
 * The SemanticStateFactory provides a centralized way to instantiate all predefined semantic states used
 * during the semantic analysis phase of the Lynx compiler. It simplifies initialization and ensures that
 * the SemanticAnalyzer has access to the complete set of analysis states.
 * 
 * **Key Responsibilities:**
 * - Construct and return all semantic states in a single collection.
 * - Encapsulate creation logic for state objects such as ClassState, InterfaceState, EnumState, etc.
 * - Ensure consistent initialization and ownership management using `std::unique_ptr`.
 * 
 * **Used By:**
 * - SemanticAnalyzer during setup to populate `activeStates`.
 * - Any testing or tooling utilities requiring a full set of semantic states.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

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
