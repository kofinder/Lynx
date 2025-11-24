/**
 * @file ArrayAccessNode.hpp
 * @brief Declares the ArrayAccessNode class representing array element access in the Lynx AST.
 * 
 * The ArrayAccessNode class handles indexing into arrays using expressions, supporting
 * LLVM IR code generation and cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores the array name and index expression.
 * - Generates LLVM IR for array element access.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - Expression nodes that access array elements.
 * 
 * @see Node, ExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_ARRAY_ACCESS_NODE_HPP
#define LYNX_ARRAY_ACCESS_NODE_HPP

#include <string>
#include "Node.hpp"
#include "ExpressionNode.hpp"

namespace LynxAst {

    using namespace LynxContext;
    using namespace LynxConstants;

    class ArrayAccessNode : public Node {

        private:

            std::string arrayName;
            std::unique_ptr<ExpressionNode> indexNode;

        public:

            explicit ArrayAccessNode(
                const std::string& arrName, 
                std::unique_ptr<ExpressionNode> idxNode
            ) : arrayName(arrName), indexNode(std::move(idxNode)) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::ARRAY_ACCESS_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~ArrayAccessNode() override = default;
    };
}
#endif
