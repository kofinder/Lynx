#include "FunctionCallNode.hpp"
#include <logger/Logger.hpp>
#include "tmpl/ImportSymbolTemplate.hpp"
#include "ExceptionHandlerNode.hpp"
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

     using namespace LynxLogger;
     using namespace LynxContext;

     llvm::Value* FunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
          LOG_WARN("Emit function call {}", functionName);

          // auto fnNode = Import::findImportSymbolNode<FunctionNode>(astContext.get(), functionName, NodeType::FUNCTION_NODE);


          std::vector<llvm::Value*> calleeArgs;
          for (auto& element: *(arguments)) {
               calleeArgs.push_back(element->generateCode(astContext->createContext()));
          }

          auto* module = astContext->getModule();
          if(auto calleFunc = module->getFunction(functionName)) {
               return generateFunctionCallIR(*astContext, calleFunc, calleeArgs);
          }

          return generateImportedFunctionCallIR(*astContext, calleeArgs);
     }

     llvm::Value* FunctionCallNode::generateImportedFunctionCallIR(const AstContext& astContext, llvm::ArrayRef<llvm::Value*> calleeArgs) {

          LOG_ERROR("Invoked ......................");

          auto* module = astContext.getModule();
          return nullptr;

          // llvm::Type* returnType = astContext->findType(fnNode->getReturnType()->type)->getLLVMType();
          // std::vector<llvm::Type*> paramTypes;
          // for (const auto& param : *fnNode->getFunctionParameter()) {
          //      auto variableType = *param->type;
          //      auto fieldType = astContext->findType(variableType)->getLLVMType();
          //      paramTypes.push_back(fieldType);
          // }
     
          // auto funcType = llvm::FunctionType::get(returnType, paramTypes, false);

          // auto calleeFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, fnNode->getQualifiedFunctionName(), module);  
          
          // std::vector<llvm::Value *> calleeArgs;
          // for (auto& element: *(arguments)) {
          //      calleeArgs.push_back(element->generateCode(astContext->createContext()));
          // }
          // return this->generateFunctionCall(astContext, calleeFunction, calleeArgs);

     }

     llvm::Value* FunctionCallNode::generateFunctionCallIR(const AstContext& astContext, llvm::Function* calleeFunction, llvm::ArrayRef<llvm::Value*> calleeArgs) {
          auto& context = astContext.getLLVMContext();
          auto& builder = astContext.getBuilder();
          auto symbolCtx = astContext.getGlobalContext();
          auto fnNode = symbolCtx->getFunctionNode();

          if(fnNode->hasExceptionHandler()) {
               auto* exceptionHandler = fnNode->getExceptionHandler();
               auto invokeCallOp = [calleeFunction, calleeArgs, &builder](llvm::BasicBlock* normalBlock, llvm::BasicBlock* exceptionBlock) -> llvm::Value* {
                    return builder.CreateInvoke(calleeFunction, normalBlock, exceptionBlock, calleeArgs);
               };  
               return exceptionHandler->handleFlow(astContext, invokeCallOp);
          }

          if(calleeFunction->getReturnType() == llvm::Type::getVoidTy(context)) {
               return builder.CreateCall(calleeFunction, calleeArgs); 
          }

          return builder.CreateCall(calleeFunction, calleeArgs, functionName);  
     }

     std::unique_ptr<Node> FunctionCallNode::clone() const {
          auto clonedArgs = std::make_unique<std::vector<std::unique_ptr<ExpressionNode>>>();
          if(arguments) {
               clonedArgs->reserve(arguments->size());
               for (const auto& arg : *arguments) {
                    if (arg) {
                        auto clonedArg = arg->clone();
                        auto exprPtr = dynamic_cast<ExpressionNode*>(clonedArg.release());
                        assert(exprPtr && "Cloned node is not an ExpressionNode");
                        clonedArgs->push_back(std::unique_ptr<ExpressionNode>(exprPtr));
                    } else {
                        clonedArgs->push_back(nullptr);
                    }
                }
        
          }
      
          auto clonedNode = std::make_unique<FunctionCallNode>(functionName, std::move(clonedArgs));
          clonedNode->setClassName(className);
          clonedNode->setObjectName(objectName);
          return clonedNode;
      }
}

