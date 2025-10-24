#ifndef LYNX_STATIC_METHOD_CALL_NODE
#define LYNX_STATIC_METHOD_CALL_NODE

#include "Node.hpp"
#include "ExpressionNode.hpp"

namespace LynxAst {

    class StaticMethodCallNode : public Node {

        private:

            std::string typeName;

            std::string methodName;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

        public:

            explicit StaticMethodCallNode(
                const std::string& type,
                const std::string& method,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ): typeName(type), methodName(method), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::STATIC_METHOD_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~StaticMethodCallNode() override = default;
    };
}

#endif 
