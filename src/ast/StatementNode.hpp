/**
 * @file StatementNode.hpp
 * @brief Declares the StatementNode class, representing a generic statement in the Lynx AST.
 * 
 * The StatementNode class models statements such as return, throw, or other operations within
 * the AST. It maintains a list of operand nodes and provides methods for LLVM IR code generation,
 * cloning, and statement-specific handling.
 * 
 * **Key Responsibilities:**
 * - Stores operands involved in the statement.
 * - Maintains the type of statement via `StatementType`.
 * - Provides LLVM IR code generation for return and throw statements.
 * - Supports deep cloning of the statement node and its operands.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR code generation for function bodies.
 * 
 * @see StatementType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_STATEMENT_NODE_HPP
#define LYNX_STATEMENT_NODE_HPP

#include "Node.hpp"
#include <vector>
#include <memory>
#include <constants/StatementType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class StatementNode : public Node {

        protected:

            std::vector<std::unique_ptr<Node>> operands;
            
            StatementType statementType;

        private:

            llvm::Value* generateReturnStatment(const AstContext& astContext);
            
            llvm::Value* generateThrowStatment(const AstContext& astContext);


        public:

            explicit StatementNode(StatementType stmtType, std::unique_ptr<Node> operand): statementType(stmtType) {
                this->operands.push_back(std::move(operand));
            }

            inline constexpr NodeType getNodeType() override { return NodeType::STATEMENT_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;

            [[nodiscard]] inline constexpr StatementType getStatementType() const { return statementType; }

            ~StatementNode() override = default;
    };

}

#endif
