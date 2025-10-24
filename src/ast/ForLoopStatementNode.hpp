#ifndef LYNX_FOR_LOOP_STATEMENT_NODE_HPP 
#define LYNX_FOR_LOOP_STATEMENT_NODE_HPP


#include "Node.hpp"
#include "StatementListNode.hpp"
#include "ExpressionNode.hpp"
#include <constants/Parameter.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class ForLoopStatementNode : public Node {
        private:
            std::unique_ptr<ExpressionNode> conditionNode;
            std::unique_ptr<StatementListNode> statementNode;
        
        public:
            std::unique_ptr<Node> preLoopNode;
            std::unique_ptr<Node> postLoopNode;
            std::unique_ptr<Node> beforeNode;
            std::unique_ptr<Node> afterNode;
        
            ForLoopStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<StatementListNode> stmts)
                : conditionNode(std::move(cond)), statementNode(std::move(stmts)) {}
        
            std::unique_ptr<Node> clone() const override;
            
            NodeType getNodeType() override { return NodeType::LOOP_NODE; }
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
        
            ~ForLoopStatementNode() override = default;
        };        
}

#endif
