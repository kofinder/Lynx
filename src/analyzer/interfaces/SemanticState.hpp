/**
 * @file SemanticState.hpp
 * @brief Declares the SemanticState class, representing a single semantic analysis state in the Lynx compiler.
 * 
 * The SemanticState class defines the interface for all analysis states used during semantic analysis.
 * Each state encapsulates specific checks or transformations on AST nodes, such as type checking,
 * symbol resolution, or validation of language rules.
 * 
 * **Key Responsibilities:**
 * - Provide a modular and extensible interface for semantic checks.
 * - Process AST nodes via `handleNode` in coordination with SemanticAnalyzer.
 * - Support polymorphic behavior for different semantic stages.
 * 
 * **Used By:**
 * - SemanticAnalyzer to manage and apply multiple analysis states.
 * - SemanticStateFactory to instantiate predefined sets of semantic states.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_ANALYZER_SEMANTIC_STATE_HPP
#define LYNX_ANALYZER_SEMANTIC_STATE_HPP

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
