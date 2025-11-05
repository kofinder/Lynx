/**
 * @file AssignmentExpressionNode.hpp
 * @brief Declares the AssignmentExpressionNode class representing assignment expressions in the Lynx AST.
 * 
 * The AssignmentExpressionNode class handles variable assignments, including simple and complex
 * assignments, operator-based assignments (e.g., +=, -=), and assignment to other assignable expressions.
 * It supports LLVM IR code generation and cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores variable name, base name, and/or assignable node.
 * - Stores the expression being assigned.
 * - Stores operator type and assignment expression type.
 * - Generates LLVM IR for simple and complex assignments.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - Expression evaluation nodes in the compiler.
 * - Code generation routines for assignment operations.
 * 
 * @see Node, AssignExpressionType, OperatorType
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_ASSIGNMENT_EXPRESSION_NODE_HPP
#define LYNX_ASSIGNMENT_EXPRESSION_NODE_HPP

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
