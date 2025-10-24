#ifndef LYNX_ASSIGNMENT_EXPRESSION_NODE_HPP
#define LYNX_ASSIGNMENT_EXPRESSION_NODE_HPP

/**
 * @file AssignmentNode.hpp
 * @brief Defines the AssignmentNode class for representing assignment operations in the AST.
 * 
 * The AssignmentNode class is used to model assignment operations within an abstract syntax tree (AST),
 * supporting assignments to variables, object properties, and array elements. It includes methods to 
 * generate the corresponding LLVM IR code for each type of assignment.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include <logger/Logger.hpp>
#include <constants/OperatorType.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>
#include <constants/expressions/AssignExpressionType.hpp>

using namespace LynxConstants;

namespace LynxAst {
    
    class AssignmentExpressionNode: public Node {

        private:

            std::string varName;
            
            std::string baseName;
            
            std::unique_ptr<Node> expressionNode; 

            std::unique_ptr<Node> assignableNode;

            AssignExpressionType assignExprType;

            OperatorType operatorType;

        public:
            
            explicit AssignmentExpressionNode(
                char* valName, 
                OperatorType oprType, 
                std::unique_ptr<Node> exprNode, 
                AssignExpressionType assignType
            ): varName(std::move(valName)), operatorType(oprType), expressionNode(std::move(exprNode)), assignExprType(assignType) {}

            explicit AssignmentExpressionNode(
                const std::string valName, 
                std::string bizName, 
                OperatorType oprType, 
                std::unique_ptr<Node> exprNode, 
                AssignExpressionType assignType
            ): varName(std::move(valName)), baseName(std::move(bizName)), operatorType(oprType), expressionNode(std::move(exprNode)), assignExprType(assignType) {}
           
            explicit AssignmentExpressionNode(
                std::unique_ptr<Node> assignNode, 
                OperatorType oprType, 
                std::unique_ptr<Node> exprNode, 
                AssignExpressionType assignType
            ): assignableNode(std::move(assignNode)), operatorType(oprType), expressionNode(std::move(exprNode)), assignExprType(assignType) {}
        
            NodeType getNodeType() override { return NodeType::ASSIGNMENT_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            std::unique_ptr<Node> clone() const override;
           
            llvm::Value* generateComplexAssign(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateSimpleAssign(std::shared_ptr<AstContext> astContext);
           
            inline std::string getVarName() const { return varName; }
            
            inline std::string getBaseName() const { return baseName;  }

            constexpr inline AssignExpressionType getAssignExprType() const { return assignExprType; }

            constexpr inline OperatorType getOperatorType() const { return operatorType; }

            inline Node* getAssignExprNode() const { return expressionNode.get(); }

            inline Node* getAssignableNode() const { return assignableNode.get(); }

            ~AssignmentExpressionNode() override = default;
    };

}

#endif
