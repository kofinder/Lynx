#ifndef LYNX_VARIABLE_DEREFERENCE_NODE_HPP
#define LYNX_VARIABLE_DEREFERENCE_NODE_HPP

/**
 * @file VariableDereferenceNode.hpp
 * @brief Class representing a variable dereference in the abstract syntax tree (AST).
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include "LiteralNode.hpp"
#include <string>
#include <memory>
#include "ArrayAccessNode.hpp"
#include "IdentifierNode.hpp"
#include <constants/DereferenceAccessType.hpp>


using namespace LynxConstants;

namespace LynxAst {

    class VariableDereferenceNode : public Node {
        
        private:
        
            std::unique_ptr<IdentifierNode> identifierAccessNode;

            std::unique_ptr<ArrayAccessNode> arrayAccessNode;

            DereferenceAccessType dereferenceType;

        public:

            static std::unique_ptr<VariableDereferenceNode> createFromIdentifier(std::unique_ptr<IdentifierNode> id) {
                auto node = std::make_unique<VariableDereferenceNode>();
                node->identifierAccessNode = std::move(id);
                node->dereferenceType = DereferenceAccessType::VARIABLE_ACCESS;
                return node;
            }
        
            static std::unique_ptr<VariableDereferenceNode> createFromArray(std::unique_ptr<ArrayAccessNode> array) {
                auto node = std::make_unique<VariableDereferenceNode>();
                node->arrayAccessNode = std::move(array);
                node->dereferenceType = DereferenceAccessType::ARRAY_ACCESS;
                return node;
            }
            
            NodeType getNodeType() override { return NodeType::VARIABLE_DEREFERENCE_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            inline constexpr DereferenceAccessType getDereferenceType() const { return dereferenceType; }

            inline IdentifierNode* getIdentifierAccessNode() const {
                if (dereferenceType != DereferenceAccessType::VARIABLE_ACCESS || !identifierAccessNode) {
                    throw std::runtime_error("VariableDereferenceNode: Not an array access node.");
                }
                return identifierAccessNode.get();
            }

            inline ArrayAccessNode* getArrayAccessNode() const {
                if (dereferenceType != DereferenceAccessType::ARRAY_ACCESS || !arrayAccessNode) {
                    throw std::runtime_error("VariableDereferenceNode: Not an array access node.");
                }
                return arrayAccessNode.get();
            }

            ~VariableDereferenceNode() override {}
    };
}

#endif
