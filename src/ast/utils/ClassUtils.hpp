#ifndef LYNX_CLASS_CODEGEN_UTILS_HPP
#define LYNX_CLASS_CODEGEN_UTILS_HPP

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <logger/Logger.hpp>
#include <ast/FunctionNode.hpp>
#include <context/AstContext.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <context/GlobalSymbolContext.hpp>


/**
 * @file ClassCodegenUtils.hpp
 * @brief Utility functions to assist with field access and manipulation for class code generation using LLVM.
 *
 * These functions help load from and store to class fields (typically from the `this` pointer) during LLVM IR generation.
 * This includes resolving field metadata, accessing field values, and assigning new values to fields.
*/
namespace LynxAst::ClassUtils {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxTypes;
    
    /**
     * @brief Resolves metadata required to access a class field.
     *
     * This includes the class struct type, the `this` pointer as a runtime value (`llvm::Value*`),
     * and the index of the field within the struct.
     *
     * @param astContext The current AST context.
     * @param fieldName The name of the field to resolve.
     * @return A tuple containing the struct type, the `this` pointer, and the field index.
     *         If resolution fails, returns nullptrs and -1.
     */
    inline std::tuple<llvm::StructType*, llvm::Value*, int> resolveFieldContext(const AstContext& astContext, const std::string& fieldName) {
        LOG_INFO("Invoked...");

        auto symbol = astContext.getGlobalContext();
        auto fnNode = symbol->getFunctionNode();

        if (!fnNode || !fnNode->clazzNode) {
            LOG_ERROR("Function node or clazzNode is null.");
            return {nullptr, nullptr, -1};
        }

        std::string className = fnNode->getCurrentClazzName();
        auto baseType = astContext.findType(className);
        if(!baseType) {
            LOG_ERROR("{} Class Not found in the context.", className);
            return {nullptr, nullptr, -1};
        }

        auto clazzType = TypeCasting::castType<ClassType>(baseType.get());
        auto* structType = llvm::cast<llvm::StructType>(clazzType->getLLVMType());

        int fieldIndex = clazzType->getFieldIndex(fieldName);
        if(fieldIndex < 0) {
            LOG_ERROR("Invalid field index '{}' for class '{}'", fieldIndex, className);
            return {nullptr, nullptr, -1};
        }

        auto* thisPtr = symbol->findLocalVariable("this");
        if(!thisPtr){
            LOG_ERROR("Attempted to access 'this' pointer, but it's null.");
            return {nullptr, nullptr, -1};
        }

        return { structType, thisPtr, fieldIndex };
    }

    inline llvm::Value* resolveSuperMethod(const AstContext& astContext, const std::string& name) {
        if(name != "super") return nullptr;

        auto symbol = astContext.getGlobalContext();
        auto fnNode = symbol->getFunctionNode();

        if (!fnNode || !fnNode->clazzNode) {
            LOG_ERROR("Function node or clazzNode is null.");
            return nullptr;
        }

        std::string className = fnNode->getCurrentClazzName();
        auto baseType = astContext.findType(className);
        if(!baseType) {
            LOG_ERROR("{} Class Not found in the context.", className);
            return nullptr;
        }

        auto* thisPtr = symbol->findLocalVariable("this");
        if(!thisPtr){
            LOG_ERROR("Attempted to access 'this' pointer, but it's null.");
            return nullptr;
        }

        if(auto clazzType = TypeCasting::castType<ClassType>(baseType.get())) {
            std::cout << "class type need to resolved name ====>:" << std::endl;
            return nullptr;
        }

        if(auto mixinType = TypeCasting::castType<MixinType>(baseType.get())) {
            auto* methodCall = symbol->getMethodCall();
            return mixinType->resolveSuperInstanceForMethod(methodCall->funcName, thisPtr, methodCall->argumentTypes);
        }

        return nullptr;
    }

    /**
     * @brief Loads a value from a field in a class struct.
     *
     * @param builder The LLVM IRBuilder to emit the load instruction.
     * @param structType The class struct type.
     * @param basePtr The pointer to the instance (`this`).
     * @param fieldIndex The index of the field in the struct.
     * @param fieldName The name of the field (used for debug naming).
     * @return The loaded field value, or nullptr if parameters are invalid.
    */
    inline llvm::Value* loadField(llvm::IRBuilder<>& builder, llvm::StructType* structType, llvm::Value* basePtr, int fieldIndex, const std::string& fieldName) {
        if (!structType || !basePtr || fieldIndex < 0 || fieldIndex >= structType->getNumElements()) {
            LOG_ERROR("Invalid load parameters for field '{}'", fieldName);
            return nullptr;
        }
    
        auto* gep = builder.CreateStructGEP(structType, basePtr, fieldIndex, fieldName + "_ptr");
        auto* fieldType = structType->getElementType(fieldIndex);
        return builder.CreateLoad(fieldType, gep, fieldName + "_val");
    }
    
    /**
     * @brief Stores a value to a field in a class struct.
     *
     * @param builder The LLVM IRBuilder to emit the store instruction.
     * @param structType The class struct type.
     * @param basePtr The pointer to the instance (`this`).
     * @param fieldIndex The index of the field in the struct.
     * @param fieldName The name of the field (used for debug naming).
     * @param valueToStore The LLVM value to store in the field.
     * @return The result of the store instruction, or nullptr if parameters are invalid.
    */
    inline llvm::Value* storeField(llvm::IRBuilder<>& builder, llvm::StructType* structType, llvm::Value* basePtr,  int fieldIndex, const std::string& fieldName, llvm::Value* valueToStore) {
        if (!structType || !basePtr || fieldIndex < 0 || fieldIndex >= structType->getNumElements()) {
            LOG_ERROR("Invalid store parameters for field '{}'", fieldName);
            return nullptr;
        }
    
        auto* gep = builder.CreateStructGEP(structType, basePtr, fieldIndex, fieldName + "_ptr");
        return builder.CreateStore(valueToStore, gep);
    }
    
    /**
     * @brief Loads a field from the current `this` instance in the current class context.
     *
     * This is a helper for accessing class fields inside instance methods.
     *
     * @param astContext The AST context for the current scope.
     * @param fieldName The name of the field to access.
     * @return The loaded field value, or nullptr if resolution fails.
    */
    inline llvm::Value* getFieldFromThis(const AstContext& astContext, const std::string& fieldName) {
        auto& builder = astContext.getBuilder();

        auto [structType, thisPtr, fieldIndex] = resolveFieldContext(astContext, fieldName);
        if (!structType || !thisPtr || fieldIndex < 0) return nullptr;

        return loadField(builder, structType, thisPtr, fieldIndex, fieldName);
    }

    /**
     * @brief Stores a value into a field of the current `this` instance.
     *
     * This is a helper for assigning values to fields inside instance methods.
     *
     * @param astContext The AST context for the current scope.
     * @param rhsValue The value to store into the field.
     * @param fieldName The name of the field to assign to.
     * @return The result of the store instruction, or nullptr if resolution fails.
    */
    inline llvm::Value* getAssignFieldFromThis(const AstContext& astContext, llvm::Value* rhsValue, const std::string& fieldName) {
        auto& builder = astContext.getBuilder();

        auto [structType, thisPtr, fieldIndex] = resolveFieldContext(astContext, fieldName);
        if (!structType || !thisPtr || fieldIndex < 0) return nullptr;

        return storeField(builder, structType, thisPtr, fieldIndex, fieldName, rhsValue);
    }

}


#endif