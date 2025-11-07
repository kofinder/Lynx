/**
 * @file SuperConstructorCallNode.hpp
 * @brief Declares the SuperConstructorCallNode class, representing a call to a superclass constructor in the Lynx AST.
 * 
 * The SuperConstructorCallNode models a call to a parent class constructor within a subclass. It holds the 
 * constructor's name and a list of arguments, allowing semantic analysis, cloning, and LLVM IR code generation.
 * 
 * **Key Responsibilities:**
 * - Stores the name of the constructor being called and its argument list.
 * - Supports LLVM IR generation for constructor calls, including argument evaluation.
 * - Provides methods for deep cloning and semantic inspection of constructor calls.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - Code generation for class instantiation in LLVM IR.
 * 
 * @see PrimaryExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_AST_SUPER_CONSTRUCTOR_CALL_NODE_HPP
#define LYNX_AST_SUPER_CONSTRUCTOR_CALL_NODE_HPP

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Node.hpp"
#include "utils/StringUtils.hpp"
#include "PrimaryExpressionNode.hpp"
#include <constants/Parameter.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class SuperConstructorCallNode : public Node {

        private:
                
            std::string construcotrName;

            std::unique_ptr<std::vector<std::unique_ptr<PrimaryExpressionNode>>> arguments;

        private:

            llvm::Value* generateExpression(const AstContext& astContext, const PrimaryExpressionNode& exprNode, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames);
        
        public:  

            explicit SuperConstructorCallNode(
                std::string ctorName, 
                std::unique_ptr<std::vector<std::unique_ptr<PrimaryExpressionNode>>> args
            ) : construcotrName(std::move(ctorName)), arguments(std::move(args)) {}
        
            inline constexpr NodeType getNodeType() override { return NodeType::SUPER_CONSTRUCTOR_CALL_NODE; }
        
            std::unique_ptr<Node> clone() const override;
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* generate(const AstContext& astContext, const ClassType& bizzType, std::vector<llvm::Value*> argOfValues, std::vector<std::string> argOfNames);

            [[nodiscard]] inline const std::string& getConstructorName() const { return construcotrName; }
                
            ~SuperConstructorCallNode() override = default;
        };

}

#endif
