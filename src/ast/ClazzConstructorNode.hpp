/**
 * @file ClazzConstructorNode.hpp
 * @brief Declares the ClazzConstructorNode class representing class constructors in the Lynx AST.
 * 
 * The ClazzConstructorNode class handles constructor declarations for user-defined classes,
 * including parameters, constructor body expressions, access modifiers, and optional super calls.
 * It supports LLVM IR code generation and cloning for AST transformations.
 * 
 * **Key Responsibilities:**
 * - Stores constructor name, parameters, body expressions, and access modifier.
 * - Handles optional super constructor calls.
 * - Generates LLVM IR for constructor code.
 * - Supports cloning for AST transformations.
 * 
 * **Used By:**
 * - ClazzDeclarationNode for class initialization.
 * - Code generation routines for constructors.
 * 
 * @see Node, Parameter, SuperConstructorCallNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CLAZZ_CONSTRUCTOR_NODE_HPP
#define LYNX_CLAZZ_CONSTRUCTOR_NODE_HPP

#include "Node.hpp"
#include <optional>
#include "ExpressionNode.hpp"
#include "SuperConstructorCallNode.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include <constants/AccessModifierType.hpp>
#include <constants/Parameter.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class ClazzConstructorNode : public Node {

        private:
        
            std::string constructorName;

            AccessModifierType accessModifierType; 

            std::unique_ptr<std::vector<std::unique_ptr<Node>>> expressions;

            std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> parameters; 

            std::optional<std::unique_ptr<SuperConstructorCallNode>> superCallNode;

        public:

           explicit ClazzConstructorNode(
                AccessModifierType type, 
                std::string cotrName, 
                std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> params, 
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> expNodes
            ) : accessModifierType(type), 
                parameters(std::move(params)), 
                constructorName(std::move(cotrName)),
                expressions(std::move(expNodes)) {}

            explicit ClazzConstructorNode(
                AccessModifierType type, 
                std::string cotrName, 
                std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> params, 
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> expNodes, 
                std::optional<std::unique_ptr<SuperConstructorCallNode>> superNode
            ) : accessModifierType(type), 
                parameters(std::move(params)), 
                constructorName(std::move(cotrName)),
                expressions(std::move(expNodes)),
                superCallNode(std::move(superNode)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::CLAZZ_CONSTRUCTOR_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            inline std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> getParameters() const { return parameters; }

            std::string getQualifiedName(const AstContext& context) const;

            const std::string& originalName() const { return constructorName; }

            ~ClazzConstructorNode() override = default;
    };

}

#endif
