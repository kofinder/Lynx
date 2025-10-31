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
                LOG_ERROR("Invoked .....");
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
