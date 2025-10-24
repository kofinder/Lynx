#ifndef LYNX_TERNARY_OPERATOR_STATEMENT_HPP
#define LYNX_TERNARY_OPERATOR_STATEMENT_HPP


#include "Node.hpp" 

using namespace LynxConstants;

namespace LynxAst {
    
    class TernaryExpressionNode: public Node {
        private:
            std::unique_ptr<Node> conditionExpression;

            std::unique_ptr<Node> thenExpression;

            std::unique_ptr<Node> endExpression;
            
        public:
            TernaryExpressionNode(std::unique_ptr<Node> conditionExpr, std::unique_ptr<Node> thenExpr, std::unique_ptr<Node> endExpr):
            conditionExpression(std::move(conditionExpr)), thenExpression(std::move(thenExpr)), endExpression(std::move(endExpr)) {}

            NodeType getNodeType() override { return NodeType::TERNARY_OPERATOR_NODE; }

            std::unique_ptr<Node> clone() const override ;

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
            
            ~TernaryExpressionNode() override = default;
    };
}

#endif
