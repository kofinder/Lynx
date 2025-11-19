/**
 * @file EnumState.hpp
 * @brief Declares the EnumState class for semantic analysis of enumeration nodes.
 * 
 * EnumState validates enum declarations in the AST, ensuring unique enumerator names,
 * proper type assignments, and correct scoping rules. It is part of the semantic
 * analysis pipeline in the Lynx compiler.
 * 
 * **Key Responsibilities:**
 * - Verify uniqueness of enum member names.
 * - Check type consistency for enumerators.
 * - Integrate enum symbols into the appropriate scope in the symbol table.
 * 
 * **Used By:**
 * - SemanticAnalyzer for parallel or sequential semantic validation.
 * - SemanticStateFactory for automatic state registration and management.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_ANALYZER_ENUM_STATE_HPP
#define LYNX_ANALYZER_ENUM_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    class EnumState : public SemanticState {

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
}

#endif 
