#ifndef LYNX_VARIABLE_UTILS_HPP
#define LYNX_VARIABLE_UTILS_HPP

#include <logger/Logger.hpp>
#include <types/tmpl/TypeChecker.hpp>
#include <context/GlobalSymbolContext.hpp>

using namespace LynxLogger;
using namespace LynxAst;
using namespace LynxContext;

namespace LynxAst::VariableUtils {

    /**
     * @brief Resolves a variable by name within the current AST context scope.
     *
     * This function performs several checks to ensure:
     * - The variable exists in the current scope.
     * - It is a valid `VariableDeclarationNode`.
     * - It has a valid LLVM IR reference (pointer).
     * - The loaded value is returned for use in codegen.
     *
     * If the variable represents an enum constant, loading may be skipped.
     *
     * @param astContext Pointer to the current AST context (includes builder, scope, etc.).
     * @param variableName Name of the variable to look up in the current scope.
     * @return llvm::Value* The LLVM IR value loaded from the variable reference.
     *         Returns nullptr if resolution fails.
     */
    inline llvm::Value* resolveVariable(AstContext* astContext, const std::string& variableName) {
        LOG_INFO("Resolving variable: {}", variableName);

        auto& builder = astContext->getBuilder();
        auto symbol = astContext->getGlobalContext();

        if (!symbol) {
            throw std::runtime_error("Scope context is null in resolveVariable()");
        }

        // Attempt to find the variable in the current scope
        Node* variableNode = symbol->findVariable(variableName);
        if (!variableNode) {
            LOG_ERROR("Variable '{}' is not defined in this scope", variableName);
            return nullptr;
        }

        // Ensure it's a valid variable declaration
        auto* variableDecl = dynamic_cast<VariableDeclarationNode*>(variableNode);
        if (!variableDecl) {
            LOG_ERROR("Invalid variable type. Expected VariableDeclarationNode.");
            return nullptr;
        }

        // Get the LLVM reference to the variable
        llvm::Value* llvmVarRef = variableDecl->getLLVMVariableRef();
        if (!llvmVarRef) {
            LOG_ERROR("LLVM variable reference is null.");
            return nullptr;
        }

        // Ensure the reference is a pointer
        if (!llvmVarRef->getType()->isPointerTy()) {
            LOG_ERROR("LLVM variable reference is not a pointer type.");
            return nullptr;
        }

        // Load the value from memory
        llvm::Type* loadType = llvmVarRef->getType()->getPointerElementType();
        if (!loadType) {
            LOG_ERROR("Failed to determine pointer element type.");
            return nullptr;
        }

        // If already a pointer to an object, no load
        if (llvmVarRef->getType()->isPointerTy() && llvmVarRef->getType()->getPointerElementType()->isStructTy()) {
            std::cerr << "already a pointer to an object, no need to load!\n";
            return llvmVarRef;
        }

        return builder.CreateLoad(loadType, llvmVarRef, variableName + "_load");
    }

    /**
     * @brief Resolves the integer tag field from an enum global constant.
     *
     * Given a global name, this function looks up the corresponding enum global
     * and retrieves its first field (usually the integer tag).
     *
     * @param globalName The full name of the enum constant (e.g., "enum.Direction.UP").
     * @param module LLVM module containing the global variable.
     * @return llvm::ConstantInt* The integer tag if successfully resolved, or nullptr.
     */
    inline llvm::ConstantInt* resolvedEnumField(const std::string& globalName, llvm::Module* module) {
        if (auto* global = module->getNamedGlobal(globalName)) {
            if (auto* structConst = llvm::dyn_cast<llvm::ConstantStruct>(global->getInitializer())) {
                auto tagConst = structConst->getOperand(0);
                if (!llvm::isa<llvm::ConstantInt>(tagConst)) {
                    return nullptr;
                }
                return llvm::cast<llvm::ConstantInt>(tagConst);
            }
        }
        return nullptr;
    }

    /**
     * @brief Resolves any field (by index) from a structured enum global constant.
     *
     * Useful when accessing the enum's tag, short value string, or full name
     * using an index:
     * - Index 0: Integer tag
     * - Index 1: Short enum value (e.g., "L")
     * - Index 2: Full name (e.g., "LEFT")
     *
     * @param globalName Full name of the enum constant.
     * @param index The struct field index to retrieve.
     * @param module LLVM module containing the global variable.
     * @return llvm::Constant* The constant field value, or nullptr if not found.
     */
    inline llvm::Constant* resolvedEnumField(const std::string& globalName, int index, llvm::Module* module) {
        if (auto* global = module->getNamedGlobal(globalName)) {
            if (auto* structConst = llvm::dyn_cast<llvm::ConstantStruct>(global->getInitializer())) {
                return structConst->getOperand(index);
            }
        }
        return nullptr;
    }


    static llvm::AllocaInst* createEntryBlockAlloca(
        llvm::IRBuilder<>& builder,
        llvm::Type* type,
        llvm::Function* function,
        const std::string& name) {
        llvm::IRBuilder<> tmp(&function->getEntryBlock(), function->getEntryBlock().begin());
        return tmp.CreateAlloca(type, nullptr, name);
    }

}

#endif
