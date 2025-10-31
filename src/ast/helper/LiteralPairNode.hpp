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
                LOG_ERROR("LiteralPairNode::generateCode .......");
                throw std::runtime_error("LiteralListNode::generateCode should be handled by parent.");        
            }

            // ✅ Const accessors
            const Node* getKey() const { return key.get(); }
            const Node* getValue() const { return value.get(); }

            // ✅ Non-const accessors (for use in type propagation)
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
