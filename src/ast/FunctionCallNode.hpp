#ifndef LYNX_FUNCTION_CALL_NODE_HPP      // Include guard to prevent multiple inclusions
#define LYNX_FUNCTION_CALL_NODE_HPP

/**
 * @file FunctionCallNode.hpp
 * @brief Defines the FunctionCallNode class, representing a function call in the AST.
 * 
 * The FunctionCallNode class models function calls within the abstract syntax tree (AST),
 * providing support for the function name, arguments, and, if applicable, object and class 
 * context for method calls. This class includes methods to generate LLVM IR for function 
 * invocations, object creation, and method calls.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 5, 2024
 */

#include "Node.hpp"
#include "FunctionNode.hpp"
#include "StatementListNode.hpp"
#include <constants/Parameter.hpp>
#include <ExpressionNode.hpp>
#include "FunctionNode.hpp"

using namespace LynxConstants;

namespace LynxAst {

    class FunctionCallNode: public Node {
        
        protected:
        
            std::string functionName;  

            std::string className;     

            std::string objectName; 
            
            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

            llvm::Value* generateFucDecleration(std::shared_ptr<AstContext> astContext, FunctionNode* fnNode);

            llvm::Value* generateFunctionCall(std::shared_ptr<AstContext> astContext, llvm::Function* calleeFunction, llvm::ArrayRef<llvm::Value*> args = llvm::None);

        public:
            
            explicit FunctionCallNode(std::string fnName): functionName(fnName) {}

            explicit FunctionCallNode(std::string fnName, std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args): functionName(fnName), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::FUNCTION_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* generateObjectMethodCallCode(std::shared_ptr<AstContext> astContext);

            inline void setArguments(std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args) { arguments = std::move(args); }

            const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const {  return *arguments; }

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> takeArguments() { return std::move(arguments); }
            
            inline void setClassName(const std::string& clazzName) { className = clazzName; }

            inline void setObjectName(const std::string& objName) { objectName = objName; }

            inline bool isObjectCreation() const { return !className.empty(); }

            inline bool isClassMethodCall() const { return !objectName.empty(); }

            inline void setFunctionName(const std::string& fnName) { functionName = fnName; }

            inline std::string getFunctionName() const { return functionName; }

            inline std::string getClazzInstanceName() const { return "new_" + objectName; }
            
            ~FunctionCallNode() override = default;
    };

} 

#endif
