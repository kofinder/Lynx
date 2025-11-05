/**
 * @file SwitchStatementNode.hpp
 * @brief Declares the SwitchStatementNode class, representing a switch-case statement in the Lynx AST.
 * 
 * The SwitchStatementNode class models a high-level switch statement, including its controlling expression
 * (the "level expression") and a collection of case blocks. It provides methods for AST traversal, cloning,
 * LLVM IR code generation, and basic semantic checks such as verifying the presence of a default block.
 * 
 * **Key Responsibilities:**
 * - Holds a controlling expression that determines which case block executes.
 * - Manages a list of `SwitchCaseBlockNode`s, representing each case in the switch statement.
 * - Supports LLVM IR generation for switch statements, including constant evaluation for case matching.
 * - Provides utility functions for semantic analysis, such as checking for a default case.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR code generation during compilation.
 * 
 * @see SwitchCaseBlockNode, SwitchLevelExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_SWITCH_STATEMENT_NODE_HPP
#define LYNX_SWITCH_STATEMENT_NODE_HPP

#include <variant>
#include "Node.hpp"
#include <constants/DataType.hpp>
#include <constants/LValueType.hpp>
#include <constants/SwitchBlockType.hpp>
#include "helper/SwitchCaseBlockNode.hpp"
#include "helper/SwitchLevelExpressionNode.hpp"

namespace LynxAst {

    using namespace LynxConstants;

    class SwitchStatementNode: public Node {

        private:

            std::unique_ptr<Node> levelExpression;

            std::unique_ptr<std::vector<std::unique_ptr<SwitchCaseBlockNode>>> basicBlocks;

            llvm::ConstantInt* tryGetCaseConstant(llvm::Value* value);
            
        public:

            SwitchStatementNode(
                std::unique_ptr<Node> levelExprNode
            ) : levelExpression(std::move(levelExprNode)),
                basicBlocks(std::make_unique<std::vector<std::unique_ptr<SwitchCaseBlockNode>>>())  {}

            NodeType getNodeType() override { return NodeType::SWITCH_NODE; }

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            void pushBasicBlock(std::unique_ptr<SwitchCaseBlockNode> node) { basicBlocks->push_back(std::move(node));  }

            inline const std::vector<std::unique_ptr<SwitchCaseBlockNode>>& getBasicBlocks() const { return *basicBlocks; }
    
            Node* getLevelExpression() const { return levelExpression.get(); }

            bool hasDefaultBlock() const;

            ~SwitchStatementNode() override = default;
    };
}

#endif
