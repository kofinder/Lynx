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
 * @date: Jan 2, 2022
*/

#ifndef LYNX_ANALYZER_ENUM_STATE_HPP
#define LYNX_ANALYZER_ENUM_STATE_HPP

#include <ast/Node.hpp>
#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    using LynxAst::Node; 

    class EnumState : public SemanticState {

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
}

#endif 
