#ifndef LYNX_SWITCH_CASE_BLOCK_HPP
#define LYNX_SWITCH_CASE_BLOCK_HPP

#include "Node.hpp"
#include <constants/SwitchBlockType.hpp>

namespace LynxAst {

    class SwitchCaseBlockNode {

        private:

            bool skipCase;

            bool fallThrough;

            SwitchBlockType caseType;
        
            std::unique_ptr<Node> exprNode;

            std::unique_ptr<Node> stmtNode;

        public:
        
            explicit SwitchCaseBlockNode(
                SwitchBlockType type,
                std::unique_ptr<Node> expr,
                std::unique_ptr<Node> stmt,
                bool _skipCase,
                bool _fallThrough
            ): caseType(type), exprNode(std::move(expr)), stmtNode(std::move(stmt)), skipCase(_skipCase), fallThrough(_fallThrough)  {}


            Node* getExpressionNode() const { return exprNode.get(); }

            Node* getStatementNode() const { return stmtNode.get(); }

            inline bool isSkipCase() const { return skipCase; }

            inline bool isFallThrough() const { return fallThrough; }

            inline const SwitchBlockType getCaseType() const { return caseType; }

            std::unique_ptr<SwitchCaseBlockNode> clone() const {
                auto clonedExpr = exprNode ? exprNode->clone() : nullptr;
                auto clonedStmt = stmtNode ? stmtNode->clone() : nullptr;
                return std::make_unique<SwitchCaseBlockNode>(caseType, std::move(clonedExpr), std::move(clonedStmt), skipCase, fallThrough);            
            }
    };
    
}

#endif