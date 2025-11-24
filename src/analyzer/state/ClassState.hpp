/**
 * @file ClassState.hpp
 * @brief Declares the ClassState class for semantic analysis of class nodes.
 * 
 * ClassState validates class declarations, including fields, methods, inheritance,
 * and access modifiers. It ensures semantic correctness within the class hierarchy.
 * 
 * **Key Responsibilities:**
 * - Verify field and method accessibility based on scope and modifiers.
 * - Validate class inheritance relationships.
 * - Ensure proper integration of class members into the symbol table.
 * 
 * **Used By:**
 * - SemanticAnalyzer during AST traversal for class nodes.
 * - SemanticStateFactory for automatic state creation and registration.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_ANALYZER_CLASS_STATE_HPP
#define LYNX_ANALYZER_CLASS_STATE_HPP

#include <unordered_map>
#include <ast/Node.hpp>
#include "interfaces/SemanticState.hpp"
#include <types/userdefined/helper/MethodType.hpp>
#include <types/userdefined/helper/FieldType.hpp>

namespace LynxAnalyzer {

    using LynxTypes::FieldType;
    using LynxTypes::MethodType;
    using LynxAst::Node; 

    class ClassState : public SemanticState {

        private:

            /**
             * @brief Checks whether a field is accessible from the given class context.
             * 
             * @param currentClass The class currently being analyzed.
             * @param field The field type being accessed.
             * @return true if accessible, false otherwise.
            */
            bool canAccessField(const std::string& currentClass, const FieldType* method) const;

            /**
             * @brief Checks whether a method is accessible from the given class context.
            */
            bool canAccessMethod(const std::string& currentClass, const MethodType* method) const;

            /**
             * @brief Determines whether `derived` is a subclass of `base`.
            */
            bool isSubclass(const std::string& derived, const std::string& base) const;

        public:
        
            /**
             * @brief Handle nodes relevant to class semantics.
             * 
             * This is called by SemanticAnalyzer when traversing AST.
            */
            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;

    };
        
}

#endif 
