/**
 * @file StatementListNode.hpp
 * @brief Declares the StatementListNode class, representing a collection of statements in the Lynx AST.
 * 
 * The StatementListNode class manages a list of statements along with their local variable declarations.
 * It provides functionality for adding statements, managing local variables, and generating LLVM IR code
 * for the complete list of statements.
 * 
 * **Key Responsibilities:**
 * - Stores a list of statement nodes in the order they appear.
 * - Maintains a mapping of local variable names to their declaration nodes.
 * - Provides methods to add statements, create locals, and search for local variables.
 * - Supports LLVM IR generation for the entire statement list.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for blocks of statements, functions, and method bodies.
 * 
 * @see VariableDeclarationNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_STATEMENT_LIST_NODE_HPP
#define LYNX_STATEMENT_LIST_NODE_HPP

#include <vector>  
#include <map>  
#include <string>  
#include "Node.hpp"
#include "VariableDeclarationNode.hpp"

namespace LynxAst {

    using namespace LynxConstants;

    class StatementListNode : public Node {  

        private:  

            std::vector<std::unique_ptr<Node>> statements;  

            std::map<std::string, VariableDeclarationNode*> locals;  

        public:  

            StatementListNode() = default;

            explicit StatementListNode(std::unique_ptr<Node> stmtNode) { 
                pushStatement(std::move(stmtNode)); 
            }

            inline constexpr NodeType getNodeType() override { return NodeType::STATEMENT_LIST_NODE; } 

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;  

            std::unique_ptr<Node> clone() const override;

            void createLocal(const std::string& varName, VariableDeclarationNode* variableNode);  

            VariableDeclarationNode* findLocal(const std::string& varName);  

            [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getStatements() const {  return statements; }
        
            [[nodiscard]] const std::map<std::string, VariableDeclarationNode*>& getLocalKeyValue() const { return locals; }

            void pushStatement(std::unique_ptr<Node> stmtNode) { statements.push_back(std::move(stmtNode)); }             

            ~StatementListNode() override = default;
            
    };  
}
#endif
