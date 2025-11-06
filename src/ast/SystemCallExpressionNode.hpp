/**
 * @file SystemCallExpressionNode.hpp
 * @brief Declares the SystemStatementNode class for system-level function calls in the AST.
 * 
 * SystemStatementNode represents calls to system or runtime functions, optionally within a specific module.
 * It supports arguments as a vector of ExpressionNode instances.
 * 
 * Responsibilities:
 * - Store the module and method names.
 * - Store arguments for the system call.
 * - Generate LLVM IR for system calls.
 * - Support cloning for AST transformations and analyses.
 * 
 * @author Ko Thein
 * @date November 4, 2024
*/

#ifndef LYNX_SYSTEM_CALL_EXPRESSION_NODE_HPP
#define LYNX_SYSTEM_CALL_EXPRESSION_NODE_HPP

#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"
#include "ExpressionNode.hpp"


namespace LynxAst {

    class SystemCallExpressionNode : public Node {

        private:

            std::string moduleName; 

            std::string methodName;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

        public:

            explicit SystemCallExpressionNode(
                std::string method,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : moduleName("system"), methodName(std::move(method)), arguments(std::move(args)) {}

            explicit SystemCallExpressionNode(
                std::string mod,
                std::string method,
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : moduleName(std::move(mod)), methodName(std::move(method)), arguments(std::move(args)) {}

            inline constexpr NodeType getNodeType() override { return NodeType::SYSTEM_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            [[nodiscard]] const std::string& getModuleName() const noexcept { return moduleName; }

            [[nodiscard]] const std::string& getMethodName() const noexcept { return methodName; }

            [[nodiscard]] const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const noexcept { return *arguments; }
            
            ~SystemCallExpressionNode() override = default;
    };
}

#endif
