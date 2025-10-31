#ifndef LYNX_PRIMARY_EXPRESSION_NODE_HPP
#define LYNX_PRIMARY_EXPRESSION_NODE_HPP

#include <string>
#include "Node.hpp"
#include "ExpressionNode.hpp"
#include "IdentifierNode.hpp"
#include <constants/expressions/PrimaryExpressionType.hpp>

using namespace LynxContext;
using namespace LynxConstants;

namespace LynxAst {

    class PrimaryExpressionNode: public Node {

        private:

            PrimaryExpressionType primaryExpType;

            std::unique_ptr<Node> innerExpNode;
    
        public:
    
            PrimaryExpressionNode(PrimaryExpressionType exprType, std::unique_ptr<Node> exprNode) : primaryExpType(exprType), innerExpNode(std::move(exprNode)) {}

            PrimaryExpressionNode(PrimaryExpressionType exprType, std::unique_ptr<IdentifierNode> identNode): primaryExpType(exprType), innerExpNode(std::move(identNode)) {}
            
            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::PRIMARY_EXPR_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            Node* getInnerExpression() const { return innerExpNode.get(); }
    
            constexpr inline PrimaryExpressionType getPrimaryExpressionType() const { return primaryExpType; }

            ~PrimaryExpressionNode() override = default;
    };
}
#endif
