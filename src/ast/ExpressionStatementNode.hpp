#ifndef LYNX_EXPRESSION_STATEMENT_NODE_HPP
#define LYNX_EXPRESSION_STATEMENT_NODE_HPP
/**
 * @file BaseType.hpp
 * @brief Abstract base class representing various data types in the AST, including methods for LLVM code generation and type management.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include <constants/Parameter.hpp>
#include <constants/expressions/ExpressionType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    /**
     * @class ExpressionNode
     * @brief Base class for various expression types in the AST, supporting unary and binary operations.
     */
    class ExpressionStatementNode: public Node {
        protected:
            std::unique_ptr<Node> expressionNode; 

        public:
            ExpressionStatementNode(std::unique_ptr<Node> operand): expressionNode(std::move(operand)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::EXPRESSION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ExpressionStatementNode() override = default;

        };
}

#endif
