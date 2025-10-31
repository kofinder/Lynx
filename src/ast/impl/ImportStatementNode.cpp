
#include <logger/Logger.hpp>
#include "ImportStatementNode.hpp"
#include "utils/StringUtils.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <libruntime/RuntimeModuleLoader.hpp>
#include <types/userdefined/FileType.hpp>
#include <constants/LinkageType.hpp>


namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxLibRuntime;
    using namespace LynxTypes;
    using namespace LynxConstants;


    llvm::Value* ImportStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Generating code for import statement: {} ", moduleName);

        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto symbol = astContext->getGlobalContext();

        RuntimeModuleType moduleType = parseModuleType(moduleName);
        bool isRuntime = RuntimeModuleLoader::loadModule(moduleType);
        if(isRuntime) {
            initializeRuntimeClassesForModule(astContext.get(), moduleType);
            return nullptr;
        }

        std::string resolvedName = StringUtils::resolveImportPath(symbol->getSources(), moduleName);
        std::string sanitizedName = "import_" + StringUtils::sanitizeIdentifier(resolvedName);

        if (symbol->hasImportedModule(resolvedName)) {
            LOG_ERROR("Module {} already imported, skipping re-import.", resolvedName);
            return nullptr;
        }

        auto importConstant = llvm::ConstantDataArray::getString(context, moduleName);
        auto importGlobal = new llvm::GlobalVariable(*module, importConstant->getType(), true, privateType, importConstant, sanitizedName);
        importGlobal->setDSOLocal(true);

        symbol->setImportFile(sanitizedName, resolvedName);

        return nullptr;
    }

    void ImportStatementNode::initializeRuntimeClassesForModule(AstContext* astContext, RuntimeModuleType moduleType) {
        LOG_INFO("This is system module {}.", moduleName);
        auto& classRegistry = RuntimeClassRegistry::getInstance();
        switch (moduleType) {
            case RuntimeModuleType::FILESYSTEM: classRegistry.resolveClass("File"); break;
            case RuntimeModuleType::COLLECTIONS:
            case RuntimeModuleType::DATETIME:
            case RuntimeModuleType::LOGGER:
            default: break;
        }
    }

    std::unique_ptr<Node> ImportStatementNode::clone() const {
        std::vector<std::unique_ptr<ImportSymbol>> clonedSymbols;
        if (namedSymbols.has_value()) {
            for (const auto& sym : *namedSymbols) {
                clonedSymbols.push_back(std::make_unique<ImportSymbol>(*sym));
            }
        }

        return std::make_unique<ImportStatementNode>(
            moduleName,
            importType,
            std::move(clonedSymbols),
            defaultImportAlias
        );
    }
    
}
