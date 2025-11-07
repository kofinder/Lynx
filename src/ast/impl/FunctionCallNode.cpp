#include "FunctionCallNode.hpp"
#include <logger/Logger.hpp>
#include "FunctionNode.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "utils/LLVMFunctionUtils.hpp"
#include "ExceptionHandlerNode.hpp"
#include <context/GlobalSymbolContext.hpp>

namespace LynxAst {

     using namespace LynxLogger;
     using namespace LynxContext;

     llvm::Value* FunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
          std::vector<llvm::Value*> calleeArgs;
          calleeArgs.reserve(arguments ? arguments->size() : 0);
          if (arguments) {
              for (auto& element : *arguments) {
                  calleeArgs.push_back(element->generateCode(astContext->createContext()));
              }
          }

          auto* module = astContext->getModule();
          if(auto calleFunc = module->getFunction(functionName)) {
               return generateFunctionCallIR(*astContext, calleFunc, calleeArgs);
          }

          return generateImportedFunctionCallIR(*astContext, calleeArgs);
     }

     llvm::Value* FunctionCallNode::generateImportedFunctionCallIR(const AstContext& astContext, llvm::ArrayRef<llvm::Value*> calleeArgs) {
          auto* module = astContext.getModule();
          auto symbolCtx = astContext.getGlobalContext();
          auto importNode = Import::findImportSymbolNode<FunctionNode>(symbolCtx.get(), functionName, NodeType::FUNCTION_NODE);

          if (!importNode.has_value()) throw std::runtime_error("Function does not exist: " + functionName);
          
          const auto* funcNode = importNode.value();
          auto* returnType = astContext.findType(funcNode->getReturnType()->type)->getLLVMType();
      
          std::vector<llvm::Type*> paramTypes;
          paramTypes.reserve(funcNode->getFunctionParameter()->size());
      
          for (const auto& param : *funcNode->getFunctionParameter()) {
              const auto& variableType = *param->type;
              paramTypes.push_back(astContext.findType(variableType)->getLLVMType());
          }
      
          auto* calleeFunc = LLVMFunction::ensureFunctionDeclared(module, returnType, paramTypes, functionName);
          return generateFunctionCallIR(astContext, calleeFunc, calleeArgs);
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