#include "FunctionCallNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include <logger/Logger.hpp>
#include "tmpl/ImportSymbolTemplate.hpp"
#include <context/VirtualTable.hpp>
#include "ExceptionHandlerNode.hpp"
#include <context/GlobalSymbolContext.hpp>


using namespace LynxLogger;
using namespace LynxContext;

namespace LynxAst {

     llvm::Value* FunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
          LOG_WARN("Generate  FunctionCallNode::generateCode {}", functionName);

          // auto fnNode = Import::findImportSymbolNode<FunctionNode>(astContext.get(), functionName, NodeType::FUNCTION_NODE);
          
          // if(fnNode.has_value()) {
          //      std::cout << "yes, it works!" << "\n";
          //      return generateFucDecleration(astContext, fnNode.value());
          // }

          // if(this->isClassMethodCall()) {
          //      return this->generateObjectMethodCallCode(astContext);
          // }

          // auto* module = astContext->getModule();
          // auto calleeFunction = module->getFunction(this->functionName);
          // if (calleeFunction == nullptr) {
          //      throw "Function not declared in the scope";
          // }

          // if (arguments == nullptr) {
          //      return this->generateFunctionCall(astContext, calleeFunction, llvm::None);
          // }

          // std::vector<llvm::Value *> calleeArgs;
          // for (auto& element: *(arguments)) {
          //      calleeArgs.push_back(element->generateCode(astContext->createContext()));
          // }

          // return this->generateFunctionCall(astContext, calleeFunction, calleeArgs);

          return nullptr;
     }

     llvm::Value* FunctionCallNode::generateFucDecleration(std::shared_ptr<AstContext> astContext, FunctionNode* fnNode) {
          LOG_WARN("Generate  FunctionCallNode::generateFucDecleration {}", functionName);
          auto* module = astContext->getModule();

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

          return nullptr;
     }


     llvm::Value* FunctionCallNode::generateObjectMethodCallCode(std::shared_ptr<AstContext> astContext) {
          LOG_WARN("[FunctionCallNode] generate Object MethodCall functionName: ({})  | objectName: ({})", functionName, objectName);
          auto* module = astContext->getModule();
          auto& context = astContext->getLLVMContext();
          auto& builder = astContext->getBuilder();
          return nullptr;
          // auto astScope = astContext->getAstContextScope();
          // auto* vNode = astScope->findVariable(objectName); // objectName should be set appropriately
          // if (!vNode) {
          //     std::cerr << "[Error] Object variable not found: " << objectName << "\n";
          //     return nullptr;
          // }
          // std::string clazzName = vNode->getVariableType()->name;
          // std::cout << "current clazz name ====>" << clazzName << std::endl;
          // auto clazzInfo = astContext->findClassInfo(clazzName);
          // if(!clazzInfo) {
          //      std::cerr << "[Error] Clazss not found: " << clazzName << "\n";
          //      return nullptr;
          // }

          // auto* funNode = clazzInfo->getFunction(functionName);
          // if(!funNode) {
          //      std::cerr << "[Error] Function not found: " << functionName << "\n";
          //      return nullptr;
          // }

          // std::string qualifiedFunctionName = funNode->getClazzFunctionName(astContext, vNode->getVariableType()->name);
          // llvm::Value* methodFn = clazzInfo->getFunctionLLVMValue(qualifiedFunctionName);
          // if(!methodFn) {
          //      std::cerr << "[Error] Qualified Function not found: " << qualifiedFunctionName << "\n";
          //      return nullptr;
          // }

          // llvm::Value* instanceVal = astContext->findInstanceVariable(getClazzInstanceName());
          // if(!instanceVal) {
          //      std::cerr << "[Error] Could not found: " << getClazzInstanceName() << "\n";
          //      return nullptr;
          // }

          // llvm::FunctionType* fnType = llvm::cast<llvm::FunctionType>(llvm::cast<llvm::PointerType>(methodFn->getType())->getPointerElementType());

          // llvm::Value* castedFn = builder.CreateBitCast(methodFn, fnType->getPointerTo(), "castFn");

          // std::vector<llvm::Value*> args;
          // args.push_back(instanceVal);
          // if (arguments) {
          //      for (auto& arg : *arguments) {
          //           llvm::Value* argVal = arg->generateCode(astContext);
          //           args.push_back(argVal);
          //      }
          // }

          // return builder.CreateCall(fnType, castedFn, args);
     }

     llvm::Value* FunctionCallNode::generateFunctionCall(std::shared_ptr<AstContext> astContext, llvm::Function* calleeFunction, llvm::ArrayRef<llvm::Value*> calleeArgs) {
          LOG_WARN("Generate  FunctionCallNode::performCall");
          auto& context = astContext->getLLVMContext();
          auto& builder = astContext->getBuilder();
          auto contextScope = astContext->getGlobalContext();
          auto fnNode = contextScope->getFunctionNode();

          if(fnNode->hasExceptionHandler()) {
               auto* exceptionHandler = fnNode->getExceptionHandler();
               return exceptionHandler->handleFlow(*astContext,[calleeFunction, calleeArgs, &builder](llvm::BasicBlock* normalBlock, llvm::BasicBlock* exceptionBlock) -> llvm::Value* {
                    return builder.CreateInvoke(calleeFunction, normalBlock, exceptionBlock, calleeArgs);
               });
          }

          if (calleeFunction->getReturnType() == llvm::Type::getVoidTy(context)) {
               return builder.CreateCall(calleeFunction, calleeArgs);
          }

          return builder.CreateCall(calleeFunction, calleeArgs, this->functionName);  
     }

     std::unique_ptr<Node> FunctionCallNode::clone() const {
          LOG_ERROR("Cloning FunctionCallNode...");
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