
#ifndef LYNX_EMPTY_NODE_HPP
#define LYNX_EMPTY_NODE_HPP

#include "Node.hpp"  

namespace LynxAst {

    class EmptyNode : public Node {

        private:
        
            llvm::Value* llvmValue;

        public:

            EmptyNode(llvm::Value* value) : llvmValue(value) {}

            std::unique_ptr<Node> clone() const override { return std::make_unique<EmptyNode>(*this); }

            NodeType getNodeType() override { return NodeType::EMPTY_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return llvmValue; }

            ~EmptyNode() override = default;
        };

}
#endif