/**
 * @file ClazzDeclarationNode.hpp
 * @brief Declares the ClazzDeclarationNode class representing class declarations in the Lynx AST.
 * 
 * The ClazzDeclarationNode class handles user-defined classes, including their fields, methods,
 * constructors, inheritance, interfaces, and mixins. It supports LLVM IR generation for class
 * types and manages virtual tables for method dispatch.
 * 
 * **Key Responsibilities:**
 * - Stores class name, fields, methods, and constructors.
 * - Handles inheritance, interfaces, and mixins.
 * - Generates LLVM IR for class definitions and member functions.
 * - Supports virtual tables for polymorphic dispatch.
 * - Provides cloning for AST transformations.
 * 
 * **Used By:**
 * - AST nodes representing class declarations.
 * - Code generation routines for user-defined types.
 * 
 * @see Node, FunctionNode, ClazzConstructorNode, ClassType, VirtualTable
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_CLAZZ_DECLARATION_NODE_HPP
#define LYNX_CLAZZ_DECLARATION_NODE_HPP


#include "Node.hpp"
#include "FunctionNode.hpp"
#include "ClazzConstructorNode.hpp"
#include <tmpl/ManglerTemplate.hpp>
#include "VariableDeclarationNode.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <types/userdefined/ClassType.hpp>
#include <constants/AccessModifierType.hpp>
#include <constants/ClassInheritanceType.hpp>

namespace LynxAst {

    using namespace LynxConstants;
    using namespace LynxContext;

    class ClazzDeclarationNode : public Node {

        private:

            std::string className;

            llvm::StructType* llvmStructType = nullptr;

            llvm::PointerType* llvmPointerType = nullptr;

            std::unique_ptr<ClassInheritanceType> inheritClass;

            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>> methods;

            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> fields;

            std::unique_ptr<std::vector<std::unique_ptr<ClazzConstructorNode>>> constructors;

            void generateDefaultCtor();

            void emitComposedMixins(const AstContext& astContext);

            void emitFields(const AstContext& astContext);

            void emitConstructors(const AstContext& astContext);

            void emitMethods(const AstContext& astContext, VirtualTable& vtable);

            void emitVTableGlobal(const AstContext& astContext, VirtualTable& vtable);

            void buildClassType(const AstContext& astContext, ClassType& classType);

            void processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members);

            std::shared_ptr<VirtualTable> emitBaseClass(const AstContext& astContext, const GlobalSymbolContext& symbol);

            void emitBaseInterfaces(const AstContext& astContext, const GlobalSymbolContext& symbol, VirtualTable& vtable);

            llvm::Function* emitMemberFunction(const AstContext& astContext, const ClassType& clazzType, FunctionNode* fnNode);

            void emitMixinMethods(
                const AstContext& astContext,
                const std::string& mangledName,
                const std::string& mixinName,
                const MethodType* method,
                unsigned int mixinOffset,
                llvm::StructType* classStructType,
                llvm::PointerType* classPointerType
            );

        public:
        
            bool isAbstract = false;

            ClazzDeclarationNode(std::string name): className(std::move(name)) {}
        
            explicit ClazzDeclarationNode(
                std::string name, 
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : className(std::move(name)) {
                if (!members || members->empty()) {  // ✅ null-safe
                    generateDefaultCtor();
                } else {
                    processMembers(std::move(members));
                }
            }
        
            explicit ClazzDeclarationNode(
                std::string name, 
                std::unique_ptr<ClassInheritanceType> bizClass,
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : className(std::move(name)), inheritClass(std::move(bizClass)) {
                if (!members || members->empty()) {  // ✅ null-safe
                    generateDefaultCtor();
                } else {
                    processMembers(std::move(members));
                }
            }

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::CLAZZ_DECLARATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
            
            inline const std::string& getOriginalName() const { return className; }

            inline bool hasInterfaces() const { return !inheritClass->interfaces.empty(); }

            inline bool hasMixins() const { return !inheritClass->mixins.empty(); }

            inline bool hasBaseClass() const { return inheritClass && inheritClass->baseClass.has_value(); }

            inline llvm::StructType* getStructType() const { return llvmStructType; }

            inline llvm::PointerType* getPointerType() { return llvmPointerType; }

            inline const std::string getQualifiedClassName() const { return Mangle::get(ManglerKind::CLASS, className); }

            inline const std::vector<std::unique_ptr<ClazzConstructorNode>>& getConstructors() const { return *constructors; }
            
            ~ClazzDeclarationNode() override = default;
    };
}

#endif
