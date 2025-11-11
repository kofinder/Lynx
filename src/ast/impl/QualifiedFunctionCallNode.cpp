#include "QualifiedFunctionCallNode.hpp"
#include "PrimaryExpressionNode.hpp"
#include <libruntime/RuntimeModuleLoader.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include "ExpressionNode.hpp"
#include "LiteralNode.hpp"
#include "utils/FileUtils.hpp"
#include <context/AstContext.hpp>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Module.h>

namespace LynxAst {

    using namespace Cloneable;
    using namespace LynxContext;
    using namespace LynxLibRuntime;

    llvm::Value* QualifiedFunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR("Namespace Name {} Function {}", qualifiedPrefixType->getRawPrefix(), funcName);
        const std::string qualifiedFunction = qualifiedPrefixType->getRawPrefix() + "::" + funcName;
        std::cout << "qualified function ======>" << qualifiedFunction << std::endl;
        auto func = RuntimeFunctionRegistry::getInstance().resolveFunction(qualifiedFunction);
        return func->call(astContext, std::move(*arguments));
    }

    std::unique_ptr<Node> QualifiedFunctionCallNode::clone() const {
        auto clonedArgs = cloneNodeVector(arguments);
        return std::make_unique<QualifiedFunctionCallNode>(
            funcName, 
            qualifiedPrefixType ? std::make_unique<QualifiedPrefixType>(*qualifiedPrefixType) : nullptr,
            std::move(clonedArgs)
        );
    }
}