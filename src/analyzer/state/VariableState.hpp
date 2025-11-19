/**
 * @file VariableState.hpp
 * @brief Declares the VariableState class, a semantic state for handling variable nodes.
 * 
 * VariableState is responsible for analyzing variable declarations, assignments, and references
 * within the AST. It integrates with the SemanticAnalyzer to ensure type correctness,
 * symbol table registration, and thread-safe analysis.
 * 
 * **Key Responsibilities:**
 * - Inspect variable declarations and assignments.
 * - Add variables to the symbol table in a thread-safe manner.
 * - Report semantic errors related to variables (e.g., redeclaration, type mismatch).
 * 
 * **Used By:**
 * - SemanticAnalyzer during parallel AST analysis.
 * - SemanticStateFactory for automatic state registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ANALYZER_VARIABLE_STATE_HPP
#define LYNX_ANALYZER_VARIABLE_STATE_HPP

#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    class VariableState : public SemanticState {

        public:
        
            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;

    };
        
}

#endif 
