/**
 * @file LiteralListNode.hpp
 * @brief Declares the LiteralListNode class representing list or array literals 
 *        in the Lynx Abstract Syntax Tree (AST).
 * 
 * The LiteralListNode class models list or array literal constructs, storing 
 * multiple element nodes that represent the contents of the list. It provides 
 * functionality for managing, cloning, and accessing contained literal elements, 
 * but does not directly handle LLVM IR generation.
 * 
 * **Key Responsibilities:**
 * - Manages a collection of literal element nodes.
 * - Supports adding, retrieving, and cloning list elements.
 * - Serves as a structural container for array-like literals in the AST.
 * 
 * **Used By:**
 * - Literal expression nodes representing lists or arrays.
 * - Code generation components handling composite literal construction.
 * 
 * @see Node, LiteralMapNode
 * 
 * @note Code generation for list literals is managed by parent nodes. 
 *       This node serves purely as an AST-level representation.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_LITERAL_LIST_NODE_HPP
#define LYNX_LITERAL_LIST_NODE_HPP

#include <variant>
#include "Node.hpp"

namespace LynxAst {

    class LiteralListNode : public Node {
        
        private:

            std::unique_ptr<std::vector<std::unique_ptr<Node>>> values;

        public:
        
            LiteralListNode() : values(std::make_unique<std::vector<std::unique_ptr<Node>>>()) {}

            std::unique_ptr<Node> clone() const override {
                auto clonedList = std::make_unique<LiteralListNode>();
                for (const auto& val : *values) {
                    clonedList->addValue(val->clone());
                }
                return clonedList;
            }

            NodeType getNodeType() override { return NodeType::LITERAL_LIST_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override {
                throw std::runtime_error("LiteralListNode::generateCode should be handled by parent.");
            }

            void addValue(std::unique_ptr<Node> val) { values->push_back(std::move(val)); }

            const auto& getValues() const { return values; }

            ~LiteralListNode() override = default;
    };
}


#endif
