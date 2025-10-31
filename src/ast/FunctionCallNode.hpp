#ifndef LYNX_FUNCTION_CALL_NODE_HPP 
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

namespace LynxAst {

    using namespace LynxConstants;

    class FunctionCallNode : public Node {
        
        protected:
        
            std::string functionName;  

            std::string className;     

            std::string objectName; 
            
            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

            llvm::Value* generateFunctionCallIR(const AstContext& astContext, llvm::Function* calleeFunction, llvm::ArrayRef<llvm::Value*> args = llvm::None);

            llvm::Value* generateImportedFunctionCallIR(const AstContext& astContext, llvm::ArrayRef<llvm::Value*> args = llvm::None);

        public:
            
            explicit FunctionCallNode(std::string fnName) : functionName(fnName) {}

            explicit FunctionCallNode(
                std::string fnName, 
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : functionName(fnName), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::FUNCTION_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            void setArguments(std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args) noexcept { arguments = std::move(args); }

            const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const {  return *arguments; }

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> takeArguments() { return std::move(arguments); }
            
            void setClassName(const std::string& clazzName) { className = clazzName; }

            void setObjectName(const std::string& objName) { objectName = objName; }

            bool isObjectCreation() const { return !className.empty(); }

            bool isClassMethodCall() const { return !objectName.empty(); }

            void setFunctionName(const std::string& fnName) { functionName = fnName; }

            const std::string& getFunctionName() const { return functionName; }

            std::string getClazzInstanceName() const { return "new_" + objectName; }

            ~FunctionCallNode() override = default;
    };

} 

#endif
