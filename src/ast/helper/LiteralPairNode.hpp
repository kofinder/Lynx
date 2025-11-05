/**
 * @file LiteralPairNode.hpp
 * @brief Declares the LiteralPairNode class representing key-value literal pairs 
 *        within collection literals in the Lynx AST.
 * 
 * The LiteralPairNode class models a single key-value pair used in literal-based 
 * data structures such as maps, dictionaries, or object-like constructs within 
 * the Lynx language. It does not directly generate LLVM IR but instead serves as 
 * a structural component managed by its parent literal node.
 * 
 * **Key Responsibilities:**
 * - Stores and manages ownership of key and value expression nodes.
 * - Provides accessors for key and value retrieval.
 * - Supports deep cloning for AST transformations.
 * 
 * **Used By:**
 * - Literal container nodes (e.g., map or dictionary literals).
 * - Syntax tree construction for composite literals.
 * 
 * @see Node, LiteralListNode
 * 
 * @note Code generation for literal pairs is handled by the parent node, not 
 *       directly within this class.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LITERAL_PAIR_NODE_HPP
#define LYNX_LITERAL_PAIR_NODE_HPP

#include "Node.hpp"

namespace LynxAst {

    class LiteralPairNode : public Node {
        
        private:

            std::unique_ptr<Node> key;

            std::unique_ptr<Node> value;

        public:

            LiteralPairNode(
                std::unique_ptr<Node> k, 
                std::unique_ptr<Node> v
            ): key(std::move(k)), value(std::move(v)) {}

            NodeType getNodeType() override { return NodeType::LITERAL_PAIR_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                throw std::runtime_error("LiteralListNode::generateCode should be handled by parent.");        
            }

            const Node* getKey() const { return key.get(); }
            const Node* getValue() const { return value.get(); }

            Node* getKey() { return key.get(); }
            Node* getValue() { return value.get(); }

            std::unique_ptr<Node> clone() const override {
                auto clonedKey = key ? key->clone() : nullptr;
                auto clonedValue = value ? value->clone() : nullptr;
                return std::make_unique<LiteralPairNode>(std::move(clonedKey), std::move(clonedValue));        
            }

            ~LiteralPairNode() override = default;
    };
}


#endif
