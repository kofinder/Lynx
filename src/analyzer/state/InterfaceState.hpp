/**
 * @file InterfaceState.hpp
 * @brief Declares the InterfaceState class, a semantic state for handling interface nodes in the AST.
 * 
 * InterfaceState is responsible for analyzing interface declarations, method and field access,
 * and inheritance relationships within the Lynx language. It integrates with SemanticAnalyzer
 * to ensure correctness of interface usage and type safety.
 * 
 * **Key Responsibilities:**
 * - Check accessibility of fields and methods according to class/interface rules.
 * - Determine subclass relationships for interface implementation checks.
 * - Integrate interface-related symbols into the semantic analysis workflow.
 * 
 * **Used By:**
 * - SemanticAnalyzer for parallel AST semantic checks.
 * - SemanticStateFactory for automatic state registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_ANALYZER_INTERFACE_STATE_HPP
#define LYNX_ANALYZER_INTERFACE_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"
#include <types/userdefined/helper/MethodType.hpp>
#include <types/userdefined/helper/FieldType.hpp>

namespace LynxAnalyzer {
    
    using namespace LynxTypes;

    class InterfaceState : public SemanticState {

        private:

            bool canAccessField(const std::string& currentClass, const FieldType* method) const;

            bool canAccessMethod(const std::string& currentClass, const MethodType* method) const;

            bool isSubclass(const std::string& derived, const std::string& base) const;

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
        
}

#endif 
