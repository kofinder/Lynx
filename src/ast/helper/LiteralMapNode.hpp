/**
 * @file LiteralMapNode.hpp
 * @brief Declares the LiteralMapNode class representing a map or dictionary literal 
 *        in the Lynx Abstract Syntax Tree (AST).
 * 
 * The LiteralMapNode class models key-value map literals, storing a collection of 
 * `LiteralPairNode` entries that represent individual key-value relationships. 
 * This node acts as a structural container and does not directly generate LLVM IR; 
 * instead, code generation is delegated to its parent node.
 * 
 * **Key Responsibilities:**
 * - Manages a list of key-value literal pairs.
 * - Provides interfaces for adding and retrieving literal pairs.
 * - Supports deep cloning for AST duplication and transformations.
 * 
 * **Used By:**
 * - Literal expression nodes representing composite data types.
 * - Code generation stages that process map or dictionary constructs.
 * 
 * @see Node, LiteralPairNode
 * 
 * @note The `generateCode` method is intentionally unimplemented, as map literal 
 *       code emission is handled at a higher AST level.
 * 
 * @authoɾ: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LITERAL_MAP_NODE_HPP
#define LYNX_LITERAL_MAP_NODE_HPP

#include "Node.hpp"
#include "LiteralPairNode.hpp"

using namespace LynxConstants;

namespace LynxAst {

    class LiteralMapNode : public Node {
        
        private:

            std::unique_ptr<std::vector<std::unique_ptr<LiteralPairNode>>> pairs;

        public:

            LiteralMapNode() : pairs(std::make_unique<std::vector<std::unique_ptr<LiteralPairNode>>>()) {}

            NodeType getNodeType() override { return NodeType::LITERAL_MAP_NODE; }

            inline const auto& getPairs() const { return pairs; }

            void addPair(std::unique_ptr<LiteralPairNode> pair) { pairs->push_back(std::move(pair)); }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                throw std::runtime_error("LiteralMapNode::generateCode should be handled by parent.");        
            }

            std::unique_ptr<Node> clone() const override {
                auto clonedMap = std::make_unique<LiteralMapNode>();
                for (const auto& pair : *pairs) {
                    if (pair) {
                        clonedMap->addPair(std::unique_ptr<LiteralPairNode>(
                            static_cast<LiteralPairNode*>(pair->clone().release()))
                        );
                    }
                }
                return clonedMap;        
            }

            ~LiteralMapNode() override = default;
    };
}


#endif
