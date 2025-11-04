#ifndef LYNX_SYSTEM_NODE_HPP
#define LYNX_SYSTEM_NODE_HPP

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"
#include "ExpressionNode.hpp"


namespace LynxAst {

    class SystemStatementNode : public Node {

        private:

            std::string moduleName; 

            std::string methodName;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

        public:

            explicit SystemStatementNode(
                std::string method,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : methodName(std::move(method)), arguments(std::move(args)) {}

            explicit SystemStatementNode(
                std::string mod,
                std::string method,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : moduleName(std::move(mod)), methodName(std::move(method)), arguments(std::move(args)) {}

            NodeType getNodeType() override { return NodeType::SYSTEM_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            [[nodiscard]] const std::string& getModuleName() const noexcept { return moduleName; }

            [[nodiscard]] const std::string& getMethodName() const noexcept { return methodName; }

            [[nodiscard]] const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const noexcept { return *arguments; }
            
            ~SystemStatementNode() override = default;
    };
}

#endif
