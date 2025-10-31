#include "LiteralNode.hpp"
#include <logger/Logger.hpp>
#include "IdentifierNode.hpp"
#include "FunctionCallNode.hpp"
#include "ClazzConstructorNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include <context/VirtualTable.hpp>
#include <constants/LinkageType.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include "tmpl/ImportSymbolTemplate.hpp"
#include "tmpl/TypeConventionTemplate.hpp"
#include "SuperConstructorCallNode.hpp"


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;
    
    llvm::Value* SuperConstructorCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("It's called unexpectedly. This method should be handled by the parent node.");
        throw std::runtime_error("It must be handled by the parent node.");    
    }

    llvm::Value* SuperConstructorCallNode::generate(
        const AstContext& astContext,
        const ClassType& bizzType, 
        std::vector<llvm::Value*> argOfValues,
        std::vector<std::string> argOfNames
    ) {   
        auto* module = astContext.getModule();
        auto& builder = astContext.getBuilder();
        auto& context = astContext.getLLVMContext();
        auto symbol = astContext.getGlobalContext();
     
        std::string clazzName = StringUtils::normalizeClassName(bizzType.originalName());
        std::string castName = StringUtils::transformLowerCase(clazzName) + "_cast";
        auto basePtrType = llvm::cast<llvm::PointerType>(bizzType.getLLVMPointerType());

        auto* thisPtr = symbol->findLocalVariable("this");
        auto* castedThis = builder.CreateBitCast(thisPtr, basePtrType, castName);

        std::vector<llvm::Value*> argValues;
        std::vector<llvm::Type*> argTypes;
        if (arguments && !arguments->empty()) {
            for (auto& arg : *arguments) {
                auto* argValue = generateExpression(astContext, *arg, argOfValues, argOfNames);
                argValues.push_back(argValue);
                argTypes.push_back(argValue->getType());
            }
        }

        auto const& exactMangled = Mangle::get(ManglerKind::CONSTRUCTOR, bizzType.originalName(), argTypes);
        auto mangledName = bizzType.resolveMethodCall(MethodKind::CONSTRUCTOR, exactMangled, argTypes);

        llvm::Function* ctorDeclFn = module->getFunction(mangledName);
        if(!ctorDeclFn) {
            argTypes.insert(argTypes.begin(), basePtrType);
            auto* functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), argTypes, false);
            ctorDeclFn = llvm::Function::Create(functionType, extType, mangledName, module);
            ctorDeclFn->setCallingConv(llvm::CallingConv::C);
        }

        llvm::errs() << "import func"; ctorDeclFn->print(llvm::outs()); llvm::errs() << "\n";

        argValues.insert(argValues.begin(), castedThis);
        return builder.CreateCall(ctorDeclFn, argValues); 
    }

    llvm::Value* SuperConstructorCallNode::generateExpression(
        const AstContext& astContext, 
        const PrimaryExpressionNode& exprNode,
        std::vector<llvm::Value*> argOfValues,
        std::vector<std::string> argOfNames
    ) {
        auto* innerExpr = exprNode.getInnerExpression();
        if (auto* identNode = dynamic_cast<IdentifierNode*>(innerExpr)) {
            for (size_t i = 0; i < argOfNames.size(); ++i) {
                if (argOfNames[i] == identNode->getName()) {
                    llvm::Value* argVal = argOfValues[i];
                    argVal->setName(identNode->getName());
                    return argVal;
                }
            }

            std::cerr << "[Error] Identifier not found in constructor args: " << identNode->getName() << "\n";
            return nullptr;
        }

        if (auto* literalNode = dynamic_cast<LiteralNode*>(innerExpr)) {
            return literalNode->generateCode(astContext.createContext());
        }
    
        if (auto* fnCallNode = dynamic_cast<FunctionCallNode*>(innerExpr)) {
            return fnCallNode->generateCode(astContext.createContext());
        }
    
        std::cerr << "[Error] Unsupported expression type in super() call\n";
        return nullptr;    
    }

    std::unique_ptr<Node> SuperConstructorCallNode::clone() const {
        auto clonedArgs = LynxAst::Cloneable::cloneNodeVector(arguments);
        auto clonedNode = std::make_unique<SuperConstructorCallNode>(construcotrName, std::move(clonedArgs));
        return clonedNode;
    } 
}


