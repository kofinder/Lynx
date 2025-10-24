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
#include "StatementListNode.hpp"
#include "ExceptionHandlerNode.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include <constants/Parameter.hpp>
#include <constants/AccessModifierType.hpp>
#include <constants/metadata/LabelTypeConstants.hpp>
#include "SuperConstructorCallNode.hpp"

using namespace LynxConstants;

namespace LynxAst {

    /**
     * @class FunctionNode
     * @brief Represents a function definition within the AST.
     * 
     * The FunctionNode class provides functionality for:
     * - Defining function names, return types, and parameters.
     * - Generating LLVM IR for the function signature and body.
     * - Managing entry and exit blocks for the function.
     */
    class FunctionNode : public Node {

        public:

            AccessModifierType accessModifier;
            bool isVirtual = false;
            bool isOverride = false;
            Node* clazzNode = nullptr;

        private:

            llvm::FunctionType* createFnSignature(AstContext& astContext);

            llvm::Value* finalizeExitBlock(AstContext& astContext);

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
            explicit FunctionNode(std::string name): fnName(name) {}

            explicit FunctionNode(std::shared_ptr<VariableType> retType, std::string name): fnName(name) {
                this->setReturnType(retType);
            }

            explicit FunctionNode(
                std::shared_ptr<VariableType> retType, 
                std::string name, 
                std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> parameters
            ): fnName(name), fnParams(std::move(parameters)) {
                this->setReturnType(retType);
            }

            std::unique_ptr<Node> clone() const override;
        
            NodeType getNodeType() override { return NodeType::FUNCTION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            llvm::Value* setReturnValue(std::shared_ptr<AstContext> astContext, llvm::Value* value);  

            inline void setFunctionBody(std::unique_ptr<StatementListNode> stmts) { this->statements = std::move(stmts); }

            inline VariableType* getReturnType() const { return returnType.get(); }

            inline const std::shared_ptr<std::vector<std::shared_ptr<Parameter>>> getFunctionParameter() const { return fnParams; }

            inline StatementListNode* getFunctionStatements() const { return statements.get(); }

            inline void setReturnType(std::shared_ptr<VariableType> retType) { this->returnType = std::move(retType); }

            inline std::string getFunctionName() const { return fnName; }

            inline llvm::Function* getLLVMFunctionRef() const { return llvmFunction; }

            inline void setLLVMFunction(llvm::Function* llvmFunc) { llvmFunction = llvmFunc;  }

            inline ExceptionHandlerNode* getExceptionHandler() {  return exceptionHandlers.top(); }

            inline void pushExceptionHandler(ExceptionHandlerNode* node) { exceptionHandlers.push(node);  }

            inline void popExceptionHandler() { this->exceptionHandlers.pop();  }

            inline bool hasExceptionHandler() const { return !this->exceptionHandlers.empty(); }

            inline void setEntryBlock(llvm::BasicBlock* block) { this->entryBlock = block; }

            inline void setExitBlock(llvm::BasicBlock* block) { this->exitBlock = block; }

            inline void setAccessModifier(AccessModifierType modifierType) { this->accessModifier = modifierType; }

            inline AccessModifierType getAccessModifier() const { return accessModifier; }

            inline void setClazzNode(Node* classNodePtr) { this->clazzNode = classNodePtr; }
    
            inline bool isClazzFunction() const { return this->clazzNode != nullptr; }

            inline void setVirtual(bool m_virtual) { this->isVirtual = m_virtual; }

            inline void setOverride(bool m_override) { this->isOverride = m_override; }

            std::string getSignatureString() const;

            std::string getCurrentClazzName() const;

            ~FunctionNode() override = default;
    };

}

#endif
