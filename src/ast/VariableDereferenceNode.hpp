/**
 * @file VariableDereferenceNode.hpp
 * @brief Declares the VariableDereferenceNode class, representing variable and array access in the AST.
 * 
 * VariableDereferenceNode models both simple variable dereferences and array element accesses.
 * It encapsulates either an IdentifierNode (for simple variable access) or an ArrayAccessNode (for array indexing),
 * and provides the interface for code generation, cloning, and type-safe access.
 * 
 * **Key Responsibilities:**
 * - Wrap identifier or array access in a uniform node type.
 * - Provide factory methods to create variable or array access nodes.
 * - Expose type-safe accessors depending on the dereference type.
 * - Support LLVM IR code generation through `generateCode`.
 * - Support AST cloning via `clone()`.
 * 
 * **Used By:**
 * - Semantic analyzer for variable and array usage.
 * - IR generator for code emission.
 * - Any compiler phase requiring unified representation of dereference operations.
 * 
 * @author Ko Thein (Nathan Mratt)
 * @date November 2, 2024
*/

#ifndef LYNX_VARIABLE_DEREFERENCE_NODE_HPP
#define LYNX_VARIABLE_DEREFERENCE_NODE_HPP

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
