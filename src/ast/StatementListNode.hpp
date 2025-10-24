#ifndef LYNX_STATEMENT_LIST_NODE_HPP
#define LYNX_STATEMENT_LIST_NODE_HPP

/**  
 * @class StatementListNode  
 *   
 * @brief Represents a node in the abstract syntax tree that contains a list of statements.  
 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 3, 2024
 */  

#include <vector>  
#include <map>  
#include <string>  
#include "Node.hpp"
#include "VariableDeclarationNode.hpp"

using namespace LynxConstants;

namespace LynxAst {

    /**
    * This class manages a collection of statement nodes and their local variable declarations.   
    * It provides functionality to add statements, manage local variables, and code generation for   
    * the statement list.        
    */

    class StatementListNode: public Node {  

        private:  

            std::vector<std::unique_ptr<Node>> statements;  

            std::map<std::string, VariableDeclarationNode*> locals;  

        public:  

            StatementListNode() = default;

            explicit StatementListNode(std::unique_ptr<Node> stmtNode) { 
                pushStatement(std::move(stmtNode)); 
            }

            NodeType getNodeType() override { return NodeType::STATEMENT_LIST_NODE; } 

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;  

            std::unique_ptr<Node> clone() const override;

            void createLocal(const std::string& varName, VariableDeclarationNode* variableNode);  

            VariableDeclarationNode* findLocal(const std::string& varName);  

            const std::vector<std::unique_ptr<Node>>& getStatements() const {  return statements; }
        
            const std::map<std::string, VariableDeclarationNode*>& getLocalKeyValue() const { return locals; }

            void pushStatement(std::unique_ptr<Node> stmtNode) { this->statements.push_back(std::move(stmtNode)); }             

            ~StatementListNode() override = default;
            
    };  
}
#endif
