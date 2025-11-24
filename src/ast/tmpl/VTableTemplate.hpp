/**
 * @file VTableTemplate.hpp
 * @brief Defines LLVM-based virtual table (vtable) emission utilities for class-like types.
 * 
 * This header provides an adaptable template-based implementation for constructing 
 * and emitting LLVM IR representations of virtual tables (vtables) for Lynx’s 
 * user-defined types — specifically classes and interfaces.
 * 
 * It abstracts away the details of LLVM structure creation, metadata association, 
 * and function mapping for virtual dispatch systems. The design supports a unified 
 * adapter-based approach that generalizes to multiple entity types (e.g., `ClassType`, 
 * `InterfaceType`, and potentially `MixinType`).
 * 
 * **Key Responsibilities:**
 * - Build LLVM `ConstantStruct` representations of vtables.
 * - Populate vtables with function pointers corresponding to virtual methods.
 * - Generate and attach metadata for reflection and debugging.
 * - Register vtable globals with LLVM’s `GlobalVariable` and COMDAT systems.
 * 
 * **Design Overview:**
 * - **Adapters (`ClassAdapter`, `InterfaceAdapter`)** abstract method name retrieval 
 *   and LLVM type access for each entity kind.
 * - **emitVTable()** constructs a fully-formed LLVM global representing the vtable 
 *   and registers it in the module.
 * 
 * **Features:**
 * - Extensible to new type categories (e.g., mixins).
 * - Supports rich metadata emission for method signatures and ordering.
 * - Integrates tightly with `VirtualTable` and `VTableType` runtime structures.
 * 
 * **Used By:**
 * - Code generation passes that emit class and interface vtables.
 * - The Lynx runtime type system for dynamic dispatch.
 * - Reflection and debug metadata generation.
 * 
 * **Functions Overview:**
 * - `emitVTable()` — Emits an LLVM vtable global for a given class/interface adapter.
 * - `ClassAdapter` — Wraps `ClassType` for vtable emission.
 * - `InterfaceAdapter` — Wraps `InterfaceType` for vtable emission.
 * 
 * @namespace LynxAst::Vtable
 * Provides template-based utilities for generating LLVM virtual tables for Lynx AST types.
 * 
 * @see VirtualTable, VTableType, ClassType, InterfaceType, LLVMContext, Module
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
 */

#ifndef LYNX_VTABLE_TEMPLATE_HPP
#define LYNX_VTABLE_TEMPLATE_HPP

#include <constants/LinkageType.hpp>
#include <context/VirtualTable.hpp>
#include <types/userdefined/ClassType.hpp>
#include <types/userdefined/helper/VTableType.hpp>
#include <types/userdefined/InterfaceType.hpp>
#include <types/userdefined/MixinType.hpp>

namespace LynxAst::Vtable {

    using namespace LynxTypes;
    using namespace LynxContext;
    using namespace LynxConstants;

    struct ClassAdapter {
        const ClassType* clazz;
        llvm::StructType* getLLVMType() const { return llvm::cast<llvm::StructType>(clazz->getLLVMType()); }
        const std::vector<std::string> getMethodList() const { return clazz->getAllVirtualParentMethodsNames(); }
        std::string getName() const { return clazz->qualifiedName(); }
    };
    
    struct InterfaceAdapter {
        const InterfaceType* iface;
        llvm::StructType* getLLVMType() const { return llvm::cast<llvm::StructType>(iface->getLLVMType()); }
        const std::vector<std::string> getMethodList() const { return iface->getMethodOrder(); }
        std::string getName() const { return iface->qualifiedName(); }
    };

    // struct MixinAdapter {
    //     const MixinType* mixin;
    //     llvm::StructType* getLLVMType() const { return llvm::cast<llvm::StructType>(mixin->getLLVMType()); }
    //     const std::vector<std::string> getMethodList() const { return mixin->getMethodOrder(); }
    //     std::string getName() const { return mixin->qualifiedName(); }
    // };

    template <typename Adapter>
    VTableType emitVTable(
        llvm::Module* module,
        llvm::LLVMContext& context,
        const std::string& qualifiedClassName,
        Adapter&& adapter,
        VirtualTable& vtable) {
            
        auto* structType = adapter.getLLVMType();
        const auto& methodNames = adapter.getMethodList();
        std::string typeName = adapter.getName();

        std::vector<llvm::Constant*> entries;
        std::vector<llvm::Metadata*> metadatas;
        std::unordered_map<std::string, unsigned> methodNameToIndex;

        unsigned index = 0;
        for (const auto& methodName : methodNames) {
            
            auto funcVal = vtable.getMethod(methodName);
            if (!funcVal) {
                llvm::errs() << "Warning: method missing in vtable: " << methodName << "\n";
                continue;  // skip missing methods
            }

            auto* implFn = llvm::cast<llvm::Function>(funcVal);
            if (!implFn) {
                llvm::errs() << "Missing method: " << methodName << "\n";
                continue;
            }

            methodNameToIndex[methodName] = index++;
            auto* expectedFnTy = structType->getElementType(entries.size());
            entries.push_back(llvm::ConstantExpr::getBitCast(implFn, expectedFnTy));
            llvm::Metadata* methodNameMD = llvm::MDString::get(context, implFn->getName().str() );
            llvm::Metadata* signatureMD = llvm::MDString::get(context, vtable.getSignature(methodName));
            metadatas.push_back(llvm::MDNode::get(context, { methodNameMD, signatureMD }));
        }

        auto vtableGlobalName = (llvm::Twine(qualifiedClassName) + "." + typeName + "_vtable").str();
        auto* vtableStruct = llvm::ConstantStruct::get(structType, entries);

        auto* vtableGlobal = new llvm::GlobalVariable(*module, structType, true, extType, vtableStruct, vtableGlobalName);
        vtableGlobal->setDSOLocal(true);
        vtableGlobal->setComdat(module->getOrInsertComdat(vtableGlobalName));

        // llvm::errs() << "v struct ===>"; structType->print(llvm::outs()); llvm::errs() << "\n";
        // llvm::errs() << "v global ===>"; vtableGlobal->print(llvm::outs()); llvm::errs() << "\n";
        // std::cout << "\n vtableGlobalName =========>" << vtableGlobalName << std::endl;

        auto* methodsNode = llvm::MDNode::get(context, metadatas);
        auto* namedMethodsNode = module->getOrInsertNamedMetadata("vtable." + qualifiedClassName + "." + typeName + ".methods");
        namedMethodsNode->addOperand(methodsNode);

        llvm::MDString* nameMD = llvm::MDString::get(context, typeName);
        llvm::Metadata* operands[] = { nameMD, methodsNode };
        llvm::MDNode* metaNode = llvm::MDNode::get(context, operands);

        module->getOrInsertNamedMetadata("lynx.vtables")->addOperand(metaNode);

        VTableType vType(vtableGlobalName, vtableGlobal, structType, methodNameToIndex);

        return vType;
    }
}

#endif 
