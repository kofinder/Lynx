/**
 * @file EmptyNode.hpp
 * @brief Declares the EmptyNode class representing a placeholder or no-operation 
 *        node in the Lynx Abstract Syntax Tree (AST).
 * 
 * The EmptyNode class acts as a minimal AST node used to represent an empty 
 * expression or statement where a syntactic placeholder is required. It can 
 * optionally hold a reference to an LLVM value for structural consistency 
 * during code generation.
 * 
 * **Key Responsibilities:**
 * - Serves as a syntactic placeholder for missing or optional nodes.
 * - Optionally stores an LLVM value reference.
 * - Supports cloning and integration within generic AST processing.
 * 
 * **Used By:**
 * - AST transformations, default branches, or syntactic placeholders.
 * - Code generation passes that require null-safe node handling.
 * 
 * @see Node, AstContext
 * 
 * @note Typically used in intermediate or fallback positions within the AST 
 *       where a node must exist but performs no operation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

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

            inline constexpr NodeType getNodeType() override { return NodeType::EMPTY_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override { return llvmValue; }

            ~EmptyNode() override = default;
        };

}
#endif