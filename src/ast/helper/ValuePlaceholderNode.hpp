/**
 * @file ValuePlaceholderNode.hpp
 * @brief Declares the ValuePlaceholderNode class representing a placeholder value node in the Lynx AST.
 * 
 * The ValuePlaceholderNode class encapsulates an LLVM value used as a temporary or intermediary
 * placeholder during code generation within the Lynx compiler's Abstract Syntax Tree (AST).
 * 
 * **Key Responsibilities:**
 * - Stores a reference to an LLVM value.
 * - Generates LLVM IR by directly returning the stored value.
 * - Supports cloning for AST transformations and optimizations.
 * 
 * **Used By:**
 * - Intermediate representation stages requiring temporary value holders.
 * - Expression evaluation and code generation passes.
 * 
 * @see Node, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_VALUE_PLACEHOLDER_NODE_HPP
#define LYNX_VALUE_PLACEHOLDER_NODE_HPP

#include "Node.hpp"

using namespace LynxConstants;

namespace LynxAst {

    class ValuePlaceholderNode : public Node {

        llvm::Value* value;

    public:

        ValuePlaceholderNode(llvm::Value* tempValue) : value(tempValue) {}

        NodeType getNodeType() override { return NodeType::VALUE_PLACEHOLDER_NODE;  }

        llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return value; }

        std::unique_ptr<Node> clone() const override { return std::make_unique<ValuePlaceholderNode>(*this); }

        ~ValuePlaceholderNode() override = default;
    };

}

#endif
