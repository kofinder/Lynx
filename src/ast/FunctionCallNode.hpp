/**
 * @file FunctionCallNode.hpp
 * @brief Declares the FunctionCallNode class, representing function and method calls in the Lynx AST.
 * 
 * The FunctionCallNode class models invocations of functions, including standalone functions,
 * class methods, and object constructors. It stores the function name, arguments, and optional
 * context such as class or object name. LLVM IR generation is supported for both local and
 * imported functions.
 * 
 * **Key Responsibilities:**
 * - Stores function metadata: name, class name, object name, and argument list.
 * - Generates LLVM IR for function or method calls, including imported functions.
 * - Provides deep cloning of the node and its arguments.
 * - Supports checks for object creation and class method calls.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for function invocation.
 * 
 * @see Node, FunctionNode, ExpressionNode, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_FUNCTION_CALL_NODE_HPP 
#define LYNX_FUNCTION_CALL_NODE_HPP

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

        private:

            llvm::Value* generateFunctionCallIR(const AstContext& astContext, llvm::Function* calleeFunction, llvm::ArrayRef<llvm::Value*> args = llvm::None);

            llvm::Value* generateImportedFunctionCallIR(const AstContext& astContext, llvm::ArrayRef<llvm::Value*> args = llvm::None);

        public:
            
            explicit FunctionCallNode(std::string fnName) : functionName(fnName) {}

            explicit FunctionCallNode(
                std::string fnName, 
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : functionName(fnName), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::FUNCTION_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            void setArguments(std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args) noexcept { arguments = std::move(args); }

            [[nodiscard]] inline const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const noexcept {  return *arguments; }

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> takeArguments() { return std::move(arguments); }
            
            void setClassName(const std::string& clazzName) { className = clazzName; }

            void setObjectName(const std::string& objName) { objectName = objName; }

            [[nodiscard]] inline bool isObjectCreation() const noexcept { return !className.empty(); }

            bool isClassMethodCall() const { return !objectName.empty(); }

            void setFunctionName(const std::string& fnName) { functionName = fnName; }

            [[nodiscard]] inline const std::string& getFunctionName() const noexcept { return functionName; }

            [[nodiscard]] inline std::string getClazzInstanceName() const noexcept { return "new_" + objectName; }

            ~FunctionCallNode() override = default;
    };

} 

#endif
