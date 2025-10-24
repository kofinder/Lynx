#ifndef LYNX_CHAINED_DEREFERENCE_NODE
#define LYNX_CHAINED_DEREFERENCE_NODE

#include "Node.hpp"
#include "VariableDereferenceNode.hpp"

using namespace LynxConstants;

namespace LynxAst {

    class ChainedDereferenceNode : public Node {

        private:
        
            std::vector<std::unique_ptr<VariableDereferenceNode>> derefChain;

        public:

            explicit ChainedDereferenceNode(std::vector<std::unique_ptr<VariableDereferenceNode>> chain) : derefChain(std::move(chain)) {}

            explicit ChainedDereferenceNode(std::unique_ptr<VariableDereferenceNode> derefNode) { derefChain.push_back(std::move(derefNode)); }

            void addDereference(std::unique_ptr<VariableDereferenceNode> derefNode) { derefChain.push_back(std::move(derefNode)); }

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::OBJECT_ACCESS_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] const auto& getChainedNode() const { return derefChain; }
    };

}
#endif 
