#ifndef LYNX_EXCEPTION_HANDLER_NODE_HPP
#define LYNX_EXCEPTION_HANDLER_NODE_HPP

/**
 * @file ExceptionHandlerNode.hpp
 * @brief Represents a node in the AST for handling try-catch-finally blocks.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
 */

#include "Node.hpp"
#include "StatementListNode.hpp"
#include <map>
#include <memory>
#include <string>
#include <functional>

namespace LynxAst {

    /**
     * @class ExceptionHandlerNode
     * @brief Manages try-catch-finally blocks in the AST, including code generation and exception handling.
     */
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

            llvm::Value* handleFlow(AstContext& astContext, std::function<llvm::Value*(llvm::BasicBlock* normalBlock, llvm::BasicBlock* exceptionBlock)> handleOperation);

            void setFinallyBlock(std::unique_ptr<StatementListNode> finallyBlock) { finallyBlockStmts = std::move(finallyBlock); }

            ~ExceptionHandlerNode() override = default;
    };

}

#endif
