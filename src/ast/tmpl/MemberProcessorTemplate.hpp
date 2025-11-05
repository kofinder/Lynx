/**
 * @file MemberProcessorTemplate.hpp
 * @brief Defines the generic `Processor` class template for organizing and cloning 
 *        class or interface member nodes in the Lynx Abstract Syntax Tree (AST).
 * 
 * The `Processor` template serves as a core utility for class-like node processing 
 * within the Lynx compiler’s AST system. It provides type-safe logic to separate, 
 * clone, and categorize class members into **methods**, **fields**, and optionally 
 * **constructors**, maintaining consistent ownership relationships and ensuring 
 * proper initialization of each node type.
 * 
 * **Key Responsibilities:**
 * - Deep clone AST member nodes for encapsulated class or interface declarations.
 * - Categorize nodes into function, variable, and constructor groups.
 * - Automatically generate default constructors when none are provided.
 * - Establish ownership relationships between class nodes and their members.
 * 
 * **Features:**
 * - Generic and reusable through the `OwnerType` template parameter.
 * - Ensures strong ownership semantics via `std::unique_ptr` cloning.
 * - Supports both constructor-aware and constructor-agnostic processing.
 * - Integrates seamlessly with the AST node hierarchy (e.g., `FunctionNode`, `VariableDeclarationNode`, `ClazzConstructorNode`).
 * 
 * **Used By:**
 * - `ClazzDeclarationNode` and other class-like AST structures.
 * - The semantic analysis and code generation stages that depend on fully 
 *   resolved class member structures.
 * 
 * @tparam OwnerType The AST node type representing the owning structure 
 *         (e.g., a class or interface declaration node).
 * 
 * @see FunctionNode, VariableDeclarationNode, ClazzConstructorNode, Node
 * 
 * @namespace LynxAst::Members
 * Provides tools for managing member nodes of class-like constructs in the Lynx AST.
 * 
 * @note Automatically injects a default public constructor if none is defined.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_MEMBER_PROCESSOR_TEMPLATE_HPP
#define LYNX_MEMBER_PROCESSOR_TEMPLATE_HPP

#include <iostream>
#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <llvm/IR/Function.h>

#include "Node.hpp"
#include "FunctionNode.hpp"
#include "VariableDeclarationNode.hpp"
#include "ClazzConstructorNode.hpp"

namespace LynxAst::Members {

    /**
     * @brief Processes class-like member nodes into categorized collections.
     * 
     * This template class centralizes logic for processing members of class or interface
     * declarations, separating them into methods, fields, and optionally constructors.
     * It clones the member nodes and sets up necessary owner references.
     * 
     * @tparam OwnerType The class type owning the members (e.g., ClazzDeclarationNode).
     */
    template<typename OwnerType>
    class Processor {
    public:
        /**
         * @brief Process members including constructors.
         * 
         * Clones members from the input vector, categorizing them into methods, fields,
         * and constructors vectors. Sets back-references on function nodes.
         * 
         * @param owner Pointer to the owning node.
         * @param members Input unique pointer to vector of member nodes.
         * @param methods Output unique pointer to vector of cloned function nodes.
         * @param fields Output unique pointer to vector of cloned variable declaration nodes.
         * @param constructors Output unique pointer to vector of cloned constructor nodes.
         */
        static void process(
            OwnerType* owner,
            const std::unique_ptr<std::vector<std::unique_ptr<Node>>>& members,
            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>>& methods,
            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>>& fields,
            std::unique_ptr<std::vector<std::unique_ptr<ClazzConstructorNode>>>& constructors
        ) {
            bool hasConstructor = false;
            methods = std::make_unique<std::vector<std::unique_ptr<FunctionNode>>>();
            fields = std::make_unique<std::vector<std::unique_ptr<VariableDeclarationNode>>>();
            constructors = std::make_unique<std::vector<std::unique_ptr<ClazzConstructorNode>>>();

            for (const auto& member : *members) {
                switch (member->getNodeType()) {
                    case NodeType::VARIABLE_DECLARATION_NODE: {
                        auto* varNode = static_cast<VariableDeclarationNode*>(member.get());
                        // varNode->setClazzNode(owner);
                        fields->push_back(std::unique_ptr<VariableDeclarationNode>(static_cast<VariableDeclarationNode*>(varNode->clone().release())));
                        break;
                    }
                    case NodeType::FUNCTION_NODE: {
                        auto* fnNode = static_cast<FunctionNode*>(member.get());
                        fnNode->setClazzNode(owner);
                        methods->push_back(std::unique_ptr<FunctionNode>(static_cast<FunctionNode*>(fnNode->clone().release())));
                        break;    
                    }
                    case NodeType::CLAZZ_CONSTRUCTOR_NODE: {
                        hasConstructor = true;
                        auto* ctorNode = static_cast<ClazzConstructorNode*>(member.get());
                        constructors->push_back(std::unique_ptr<ClazzConstructorNode>(static_cast<ClazzConstructorNode*>(ctorNode->clone().release())));
                        break;
                    }
                    default:
                        LOG_ERROR("Unexpected node type");
                        break;
                }
            }

            if (!hasConstructor) {
                auto defaultCtor = std::make_unique<ClazzConstructorNode>(
                    AccessModifierType::PUBLIC, 
                    owner->getOriginalName(), 
                    std::make_shared<std::vector<std::shared_ptr<Parameter>>>(), 
                    std::make_unique<std::vector<std::unique_ptr<Node>>>()
                );
                constructors->push_back(std::move(defaultCtor));
            }
        }

        /**
         * @brief Process members excluding constructors.
         * 
         * Similar to the above method but only processes methods and fields.
         * 
         * @param owner Pointer to the owning node.
         * @param members Input unique pointer to vector of member nodes.
         * @param methods Output unique pointer to vector of cloned function nodes.
         * @param fields Output unique pointer to vector of cloned variable declaration nodes.
         */
        static void process(
            OwnerType* owner,
            const std::unique_ptr<std::vector<std::unique_ptr<Node>>>& members,
            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>>& methods,
            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>>& fields
        ) {
            methods = std::make_unique<std::vector<std::unique_ptr<FunctionNode>>>();
            fields = std::make_unique<std::vector<std::unique_ptr<VariableDeclarationNode>>>();

            for (const auto& member : *members) {
                switch (member->getNodeType()) {
                    case NodeType::VARIABLE_DECLARATION_NODE: {
                        auto* varNode = static_cast<VariableDeclarationNode*>(member.get());
                        fields->push_back(std::unique_ptr<VariableDeclarationNode>(static_cast<VariableDeclarationNode*>(varNode->clone().release())));
                        break;
                    }
                    case NodeType::FUNCTION_NODE: {
                        auto* fnNode = static_cast<FunctionNode*>(member.get());
                        fnNode->setClazzNode(owner);
                        methods->push_back(std::unique_ptr<FunctionNode>(static_cast<FunctionNode*>(fnNode->clone().release())));
                        break;    
                    }
                    default:
                        LOG_ERROR("Unexpected node type");
                        break;
                }
            }
        }
    };

}

#endif
