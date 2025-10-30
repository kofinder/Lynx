#ifndef LYNX_FUNCTION_NODE_HPP
#define LYNX_FUNCTION_NODE_HPP

/**
 * @file FunctionNode.hpp
 * @brief Defines the FunctionNode class, representing a function definition in the AST.
 * 
 * The FunctionNode class models function definitions within the abstract syntax tree (AST),
 * providing support for function name, return type, parameters, and body statements. 
 * This class includes methods to generate LLVM IR for function entry, exit, and signature.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include <optional>
#include <stack>
#include "StatementListNode.hpp"
#include "ExceptionHandlerNode.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include <constants/Parameter.hpp>
#include "SuperConstructorCallNode.hpp"
#include <constants/AccessModifierType.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>

namespace LynxAst {

    using namespace LynxConstants;

    class FunctionNode : public Node {

        public:

            AccessModifierType accessModifier;
            bool isVirtual = false;
            bool isOverride = false;
            Node* clazzNode = nullptr;

        private:

            llvm::FunctionType* createFnSignature(const AstContext& astContext);

            llvm::Value* finalizeExitBlock(const AstContext& astContext);

        protected:

            std::string fnName; 
            std::shared_ptr<VariableType> returnType; 
            std::unique_ptr<StatementListNode> statements;
            std::stack<ExceptionHandlerNode*> exceptionHandlers;
            std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> fnParams;

            llvm::BasicBlock* entryBlock = nullptr;
            llvm::BasicBlock* exitBlock = nullptr;
            llvm::Function* llvmFunction = nullptr;
            llvm::AllocaInst* returnValue = nullptr;

        public:

            explicit FunctionNode(std::string name) : fnName(name) {}

            explicit FunctionNode(std::shared_ptr<VariableType> retType, std::string name) : fnName(name) { setReturnType(retType); }

            explicit FunctionNode(
                std::shared_ptr<VariableType> retType, 
                std::string name, 
                std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> parameters
            ) : fnName(name), fnParams(std::move(parameters)) { setReturnType(retType); }

            std::unique_ptr<Node> clone() const override;
        
            NodeType getNodeType() override { return NodeType::FUNCTION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* setReturnValue(std::shared_ptr<AstContext> astContext, llvm::Value* value);  

            void setFunctionBody(std::unique_ptr<StatementListNode> stmts) { statements = std::move(stmts); }

            [[nodiscard]] VariableType* getReturnType() const noexcept { return returnType.get(); }

            [[nodiscard]] const std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> getFunctionParameter() const noexcept { return fnParams; }

            StatementListNode* getFunctionStatements() const noexcept { return statements.get(); }

            void setReturnType(std::shared_ptr<VariableType> retType) { returnType = std::move(retType); }

            std::string getFunctionName() const noexcept { return fnName; }

            llvm::Function* getLLVMFunctionRef() const noexcept { return llvmFunction; }

            void setLLVMFunction(llvm::Function* llvmFunc) noexcept { llvmFunction = llvmFunc;  }

            ExceptionHandlerNode* getExceptionHandler() const noexcept {  return exceptionHandlers.empty() ? nullptr : exceptionHandlers.top(); }

            void pushExceptionHandler(ExceptionHandlerNode* node) noexcept { exceptionHandlers.push(node);  }

            void popExceptionHandler() noexcept { exceptionHandlers.pop();  }

            bool hasExceptionHandler() const noexcept { return !exceptionHandlers.empty(); }

            void setEntryBlock(llvm::BasicBlock* block) noexcept { entryBlock = block; }

            void setExitBlock(llvm::BasicBlock* block) noexcept { exitBlock = block; }

            void setAccessModifier(AccessModifierType modifierType) noexcept { accessModifier = modifierType; }

            AccessModifierType getAccessModifier() const noexcept { return accessModifier; }

            void setClazzNode(Node* classNodePtr) noexcept { clazzNode = classNodePtr; }
    
            bool isClazzFunction() const noexcept { return clazzNode != nullptr; }

            void setVirtual(bool value) noexcept { isVirtual = value; }

            void setOverride(bool value) noexcept { isOverride = value; }

            std::string getSignatureString() const;

            std::string getCurrentClazzName() const;

            ~FunctionNode() override = default;
    };

}

#endif
