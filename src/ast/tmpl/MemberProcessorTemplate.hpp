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
