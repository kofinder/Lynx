/**
 * @file SwitchCaseBlock.hpp
 * @brief Declares the SwitchCaseBlockNode class representing a single case or default 
 *        block within a switch statement in the Lynx AST.
 * 
 * The SwitchCaseBlockNode class encapsulates the components of a switch-case block, 
 * including its case type (normal, default, or skipped), associated expression, and 
 * statement body. It also tracks fall-through and skip-case behaviors during 
 * switch-case code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the case expression and statement body.
 * - Distinguishes between different switch block types (case, default, skipped).
 * - Maintains flags for fall-through and skip-case handling.
 * - Supports deep cloning for AST duplication and transformation.
 * 
 * **Used By:**
 * - Switch statement construction and LLVM IR generation.
 * - Control-flow handling in the compiler frontend.
 * 
 * @see Node, SwitchBlockType
 * 
 * @note Each case block may optionally skip evaluation or fall through to subsequent cases,
 *       depending on compiler configuration or user-defined syntax.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

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