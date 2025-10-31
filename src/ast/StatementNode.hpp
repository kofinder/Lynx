#ifndef LYNX_STATEMENT_NODE_HPP
#define LYNX_STATEMENT_NODE_HPP

/**
 * @file StatementNode.hpp
 * @brief Class representing a statement node in an abstract syntax tree (AST).
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include <vector>
#include <memory>
#include <constants/StatementType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class StatementNode : public Node {

        protected:

            std::vector<std::unique_ptr<Node>> operands;
            
            StatementType statementType;

        public:

            explicit StatementNode(StatementType stmtType, std::unique_ptr<Node> operand): statementType(stmtType) {
                this->operands.push_back(std::move(operand));
            }

            NodeType getNodeType() override { return NodeType::STATEMENT_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            llvm::Value* generateReturnStatment(std::shared_ptr<AstContext> astContext);
        
            llvm::Value* generateThrowStatment(std::shared_ptr<AstContext> astContext);

            inline constexpr StatementType getStatementType() const { return statementType; }

            ~StatementNode() override = default;
    };

}

#endif
