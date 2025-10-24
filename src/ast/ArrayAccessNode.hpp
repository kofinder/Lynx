#ifndef LYNX_ARRAY_ACCESS_NODE_HPP
#define LYNX_ARRAY_ACCESS_NODE_HPP

#include <string>
#include "Node.hpp"
#include "ExpressionNode.hpp"
using namespace LynxContext;
using namespace LynxConstants;

namespace LynxAst {

    class ArrayAccessNode: public Node {

        private:
            std::string arrayName;
            std::unique_ptr<ExpressionNode> indexNode;

        public:
            explicit ArrayAccessNode(const std::string& arrName, std::unique_ptr<ExpressionNode> idxNode) : arrayName(arrName), indexNode(std::move(idxNode)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::ARRAY_ACCESS_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ArrayAccessNode() override = default;
    };
}
#endif
