/**
 * @file FunctionState.hpp
 * @brief Declares the FunctionState class, responsible for semantic analysis of function nodes.
 * 
 * FunctionState processes function declarations and definitions in the AST, verifying
 * parameters, return types, and scope rules. It integrates with SemanticAnalyzer to enforce
 * function-level semantic correctness in the Lynx language.
 * 
 * **Key Responsibilities:**
 * - Validate function signatures, parameter types, and return types.
 * - Ensure proper scoping of local variables and parameters.
 * - Integrate function symbols into the global or class-level symbol tables.
 * 
 * **Used By:**
 * - SemanticAnalyzer for parallel or sequential AST semantic checks.
 * - SemanticStateFactory for automatic state registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_ANALYZER_FUNCTION_STATE_HPP
#define LYNX_ANALYZER_FUNCTION_STATE_HPP

#include <ast/Node.hpp>
#include "interfaces/SemanticState.hpp"

namespace LynxAnalyzer {

    using LynxAst::Node;

    class FunctionState : public SemanticState {

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    
    };
}

#endif 
