#include "QualifiedFunctionCallNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "utils/VariableUtils.hpp"
#include <resolver/TypeResolverFactory.hpp>
#include <libruntime/RuntimeModuleLoader.hpp>

namespace LynxAst {

    using namespace Cloneable;
    using namespace LynxContext;
    using namespace LynxLibRuntime;
    using namespace VariableUtils;
    using LynxResolver::TypeResolverFactory;

    llvm::Value* QualifiedFunctionCallNode::generateCode(std::shared_ptr<AstContext> astContext) {

        LOG_ERROR("Namespace Name {} Function {}", qualifiedPrefixType->getRawPrefix(), funcName);
        const std::string qualifiedFunction = qualifiedPrefixType->getRawPrefix() + "::" + funcName;
        std::cout << "qualified function ======>" << qualifiedFunction << std::endl;
        
        if(auto func = RuntimeFunctionRegistry::getInstance().resolveFunction(qualifiedFunction)) {
            return func->call(astContext, std::move(*arguments));
        }
        
        return dispatchInstanceExtensionMethod(std::move(astContext));
    }

    llvm::Value* QualifiedFunctionCallNode::dispatchInstanceExtensionMethod(std::shared_ptr<AstContext> astContext) {
        LOG_ERROR(" Name {} Function {}", qualifiedPrefixType->getRawPrefix(), funcName);

        const std::string& varName = qualifiedPrefixType->getRawPrefix();
        LOG_ERROR("Dispatching instance extension method for '{}'", varName);

        auto resolvedVar = VariableUtils::resolveVariable(*astContext, varName);
        if (!resolvedVar.value) {
            LOG_ERROR("Failed to resolve variable '{}'", varName);
            return nullptr;
        }

        auto dataType = astContext->findType(resolvedVar.value)->getTypeTag();
        auto resolver = TypeResolverFactory::forType(dataType);
        if (!resolver) {
            LOG_ERROR("Unknown type resolver for type: {}", qualifiedPrefixType->getRawPrefix());
            return nullptr;
        }

        std::vector<llvm::Value*> argValues;
        argValues.reserve(arguments->size());
        for (auto& arg : *arguments) {
            llvm::Value* value = arg->generateCode(astContext);
            argValues.push_back(value);
        }

        auto* result = resolver->resolveMethod(funcName, resolvedVar.reference, argValues, std::move(astContext));

        if (!result) {
            LOG_ERROR("Failed to resolve static method '{}.{}'", qualifiedPrefixType->getRawPrefix(), funcName);
            return nullptr;
        }

        return result;
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