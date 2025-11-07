#include "QualifiedIdentifierNode.hpp"
#include <context/GlobalSymbolContext.hpp>
#include "tmpl/CloneNodeTemplate.hpp"
#include <constants/LinkageType.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <logger/Logger.hpp>

namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxTypes;
    using namespace LynxConstants;

    constexpr std::string_view enum_prefix_namespace = "enum";

    llvm::Value* QualifiedIdentifierNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation  ....{}", identifierName);

        auto systemType = qualifiedPrefixType->getPrefixType();

        if(systemType == SystemPrefixType::OTHERS) {
            return emitEnumValue(*astContext);
        }
        
        return emitSystemValue(*astContext);
    }

    llvm::Value* QualifiedIdentifierNode::emitEnumValue(const AstContext& astContext) {

        auto* module = astContext.getModule();

        const auto qualifiedName = qualifiedPrefixType->getRawPrefix();
        auto enumGlobalName = (llvm::Twine(enum_prefix_namespace) + "." + qualifiedName + "." + identifierName).str();

        // Get global const enum value from single module
        if (auto* value = module->getNamedGlobal(enumGlobalName)) {
            return value;
        }

        // Get global const enum value frim cross module
        auto baseType = astContext.findType(qualifiedName);
        if(auto enumType = TypeCasting::castType<EnumType>(baseType.get())) {
            // declaration only for enum constant
            auto gvalue = new llvm::GlobalVariable(*module, enumType->getLLVMType(), true, extType, nullptr, enumGlobalName);

            return gvalue;
        }

        LOG_ERROR("Global not found for enum: {}", enumGlobalName);

        return nullptr;        
    }

    llvm::Value* QualifiedIdentifierNode::emitSystemValue(const AstContext& astContext) {
        LOG_ERROR("Emit System Value ................!");
        return nullptr;
    }

    std::unique_ptr<Node> QualifiedIdentifierNode::clone() const {
        auto clonedPrefixType = std::make_unique<QualifiedPrefixType>(*qualifiedPrefixType);
        auto clonedNode = std::make_unique<QualifiedIdentifierNode>(identifierName, std::move(clonedPrefixType));
        return clonedNode;
    }
}
