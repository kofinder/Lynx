#include "FunctionNode.hpp"
#include <logger/Logger.hpp>
#include <context/GlobalSymbolContext.hpp>
#include "ExceptionHandlerNode.hpp"


namespace LynxAst {
    
    using namespace LynxLogger;
    using namespace LynxContext;

    llvm::Value* ExceptionHandlerNode::generateCode(std::shared_ptr<AstContext> astContext) {
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        auto scopeContext = astContext->getGlobalContext();
        auto fncNode = scopeContext->getFunctionNode();

        // Unwind block for invoke
        exceptionBlock = llvm::BasicBlock::Create(context, "exception", fncNode->getLLVMFunctionRef());

        // finally block
        auto* afterExceptionBlock = llvm::BasicBlock::Create(context, "afterExceptionBlock", fncNode->getLLVMFunctionRef());

        // try block
        fncNode->pushExceptionHandler(this);
        this->tryBlockStmts->generateCode(astContext->createContext());
        fncNode->popExceptionHandler();

        // Ending last normal block
        if (builder.GetInsertBlock()->getTerminator() == nullptr) {
           builder.CreateBr(afterExceptionBlock);
        }

        // catch block
        builder.SetInsertPoint(exceptionBlock);
        std::vector<llvm::Type *> caughtResultFieldTypes = {
            builder.getInt8PtrTy(),
            builder.getInt32Ty()
        };

        auto* ourCaughtResultType = llvm::StructType::get(context, caughtResultFieldTypes);
        auto* caughtResult = builder.CreateLandingPad(ourCaughtResultType, 1, "landingPad");
        auto ptr = builder.CreateBitCast(module->getGlobalVariable("_ZTIi"), builder.getInt8PtrTy());
        caughtResult->addClause(static_cast<llvm::Constant *>(ptr));

        // TODO properly
        catchBlocksStmts.begin()->second->generateCode(astContext->createContext());

        // TODO properly
        if (finallyBlockStmts != nullptr && exceptionBlock->getTerminator() == nullptr) {
            finallyBlockStmts->generateCode(astContext->createContext());
        }
            
        if (exceptionBlock->getTerminator() == nullptr) {
            builder.CreateBr(afterExceptionBlock);
        }

       builder.SetInsertPoint(afterExceptionBlock);

        return nullptr;
    }


    llvm::Value* ExceptionHandlerNode::handleFlow(const AstContext& astContext, std::function<llvm::Value*(llvm::BasicBlock* normalBlock, llvm::BasicBlock* exceptionBlock)> handleOperation) {
        auto& context = astContext.getLLVMContext();
        auto& builder = astContext.getBuilder();
        auto scopeContext = astContext.getGlobalContext();
        auto fncNode = scopeContext->getFunctionNode();

        auto nb = llvm::BasicBlock::Create(context, "normal", fncNode->getLLVMFunctionRef());
        auto* value = handleOperation(nb, this->exceptionBlock);
        builder.SetInsertPoint(nb);

        return value;
    }

    std::unique_ptr<Node> ExceptionHandlerNode::clone() const {
        // Clone the try block
        std::unique_ptr<StatementListNode> clonedTryBlock = tryBlockStmts
            ? std::unique_ptr<StatementListNode>(
                static_cast<StatementListNode*>(tryBlockStmts->clone().release()))
            : nullptr;
    
        // Create new ExceptionHandlerNode with the cloned try block
        auto clonedNode = std::make_unique<ExceptionHandlerNode>(std::move(clonedTryBlock));
    
        // Clone the finally block
        if (finallyBlockStmts) {
            std::unique_ptr<StatementListNode> clonedFinallyBlock(
                static_cast<StatementListNode*>(finallyBlockStmts->clone().release()));
            clonedNode->setFinallyBlock(std::move(clonedFinallyBlock));
        }
    
        // Clone each catch block
        for (const auto& [typePtr, stmtList] : catchBlocksStmts) {
            if (stmtList) {
                std::unique_ptr<StatementListNode> clonedCatchBlock(
                    static_cast<StatementListNode*>(stmtList->clone().release()));
                // Note: We don't have the exceptionName stored in this structure.
                // You may need to add that to the class if needed during cloning.
                clonedNode->addCatchBlock(std::move(clonedCatchBlock), std::shared_ptr<VariableType>(typePtr), "");
            }
        }
    
        return clonedNode;
    }
}