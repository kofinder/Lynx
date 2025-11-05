/**
 * @file ExceptionHandlerNode.hpp
 * @brief Declares the ExceptionHandlerNode class, representing try-catch-finally blocks in the Lynx AST.
 * 
 * The ExceptionHandlerNode class models exception handling constructs, including try blocks, 
 * multiple catch blocks, and an optional finally block. It provides LLVM IR generation, deep cloning, 
 * and mechanisms for managing control flow during exceptions.
 * 
 * **Key Responsibilities:**
 * - Stores try block, multiple catch blocks, and finally block statements.
 * - Supports flow handling for normal and exceptional control paths.
 * - Generates LLVM IR for exception handling.
 * - Provides deep cloning for AST transformations.
 * 
 * **Used By:**
 * - AST construction and semantic analysis.
 * - LLVM IR generation for exception handling constructs.
 * 
 * @see Node, StatementListNode, AstContext
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_EXCEPTION_HANDLER_NODE_HPP
#define LYNX_EXCEPTION_HANDLER_NODE_HPP


#include "Node.hpp"
#include "StatementListNode.hpp"
#include <map>
#include <memory>
#include <string>
#include <functional>

namespace LynxAst {

    class ExceptionHandlerNode : public Node {
        
        private:
        
            std::unique_ptr<StatementListNode> tryBlockStmts; 

            std::unique_ptr<StatementListNode> finallyBlockStmts;

            llvm::BasicBlock* exceptionBlock = nullptr;  

            std::map<std::shared_ptr<VariableType>, std::unique_ptr<StatementListNode>> catchBlocksStmts;  

        public:
            
            explicit ExceptionHandlerNode(std::unique_ptr<StatementListNode> tryBlock) : tryBlockStmts(std::move(tryBlock)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::EXCEPTION_HANDLE_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            void addCatchBlock(
                std::unique_ptr<StatementListNode> catchBlock, 
                std::shared_ptr<VariableType> exceptionType, 
                const std::string& exceptionName) {
                catchBlocksStmts[exceptionType] = std::move(catchBlock);
            }

            llvm::Value* handleFlow(const AstContext& astContext, std::function<llvm::Value*(llvm::BasicBlock* normalBlock, llvm::BasicBlock* exceptionBlock)> handleOperation);

            void setFinallyBlock(std::unique_ptr<StatementListNode> finallyBlock) { finallyBlockStmts = std::move(finallyBlock); }

            ~ExceptionHandlerNode() override = default;
    };

}

#endif
