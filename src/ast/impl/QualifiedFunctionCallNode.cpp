#include "QualifiedFunctionCallNode.hpp"
#include "PrimaryExpressionNode.hpp"
#include <libruntime/RuntimeModuleLoader.hpp>
#include "ExpressionNode.hpp"
#include "LiteralNode.hpp"
#include "utils/FileUtils.hpp"
#include <context/AstContext.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Module.h>


using namespace LynxContext;
using namespace LynxLibRuntime;

namespace LynxAst {

    llvm::Value* QualifiedFunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Namespace Name {} Function {}", qualifiedPrefixType->getRawPrefix(), funcName);
        const std::string qualifiedFunction = qualifiedPrefixType->getRawPrefix() + "::" + funcName;
        std::cout << "qualified function ======>" << qualifiedFunction << std::endl;
        auto func = RuntimeFunctionRegistry::getInstance().resolveFunction(qualifiedFunction);
        return func->call(astContext, std::move(*arguments));
    }

    std::unique_ptr<Node> QualifiedFunctionCallNode::clone() const {
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

        auto clonedNode = std::make_unique<QualifiedFunctionCallNode>(
            funcName, 
            qualifiedPrefixType ? std::make_unique<QualifiedPrefixType>(*qualifiedPrefixType) : nullptr,
            std::move(clonedArgs)
        );
        return clonedNode;
    }
}