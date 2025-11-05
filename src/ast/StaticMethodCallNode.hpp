/**
 * @file StaticMethodCallNode.hpp
 * @brief Declares the StaticMethodCallNode class, representing a static method call in the Lynx AST.
 * 
 * The StaticMethodCallNode class models a call to a static method of a class, including the type name,
 * method name, and a list of argument expressions. It supports AST traversal, cloning, and LLVM IR code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the type of the class and the static method being called.
 * - Maintains a list of arguments for the method call.
 * - Provides LLVM IR code generation for static method calls.
 * - Supports deep cloning of the node and its arguments.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation during compilation.
 * 
 * @see ExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

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
