#ifndef LYNX_SWITCH_STATEMENT_NODE_HPP
#define LYNX_SWITCH_STATEMENT_NODE_HPP

#include <variant>
#include "Node.hpp"
#include <constants/DataType.hpp>
#include <constants/LValueType.hpp>
#include <constants/SwitchBlockType.hpp>
#include "helper/SwitchCaseBlockNode.hpp"
#include "helper/SwitchLevelExpressionNode.hpp"


using namespace LynxConstants;

namespace LynxAst {

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
