#ifndef ABCB48D6_C49D_421F_B36F_AACFFEFA30B8
#define ABCB48D6_C49D_421F_B36F_AACFFEFA30B8
#ifndef LYNX_LITERAL_LIST_NODE_HPP
#define LYNX_LITERAL_LIST_NODE_HPP

#include <variant>
#include "Node.hpp"

namespace LynxAst {

    class LiteralListNode : public Node {
        
        private:

            std::unique_ptr<std::vector<std::unique_ptr<Node>>> values;

        public:
        
            LiteralListNode() : values(std::make_unique<std::vector<std::unique_ptr<Node>>>()) {}

            std::unique_ptr<Node> clone() const override {
                auto clonedList = std::make_unique<LiteralListNode>();
                for (const auto& val : *values) {
                    clonedList->addValue(val->clone());
                }
                return clonedList;
            }

            NodeType getNodeType() override { return NodeType::LITERAL_LIST_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                LOG_ERROR("Invoked.");
                throw std::runtime_error("LiteralListNode::generateCode should be handled by parent.");
            }

            void addValue(std::unique_ptr<Node> val) { values->push_back(std::move(val)); }

            const auto& getValues() const { return values; }

            ~LiteralListNode() override = default;
    };
}


#endif


#endif /* ABCB48D6_C49D_421F_B36F_AACFFEFA30B8 */
