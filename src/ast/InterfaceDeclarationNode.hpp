/**
 * @file InterfaceDeclarationNode.hpp
 * @brief Declares the InterfaceDeclarationNode class, representing interface declarations in the Lynx AST.
 * 
 * The InterfaceDeclarationNode class models user-defined interfaces, including their fields, methods,
 * and inherited interfaces. It supports semantic analysis, LLVM IR generation, and virtual table management
 * for interface method dispatch.
 * 
 * **Key Responsibilities:**
 * - Stores the interface name, fields, methods, and parent interfaces.
 * - Processes member nodes to separate methods and fields.
 * - Generates LLVM struct and pointer types for the interface.
 * - Emits LLVM IR for member functions and global vtables.
 * - Supports deep cloning of the node and its members.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for interface types and virtual method calls.
 * 
 * @see FunctionNode, VariableDeclarationNode, InterfaceType, VirtualTable, Mangle
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_INTERFACE_DECLARATION_NODE_HPP
#define LYNX_INTERFACE_DECLARATION_NODE_HPP

#include "Node.hpp"
#include "FunctionNode.hpp"
#include <tmpl/ManglerTemplate.hpp>
#include "ClazzConstructorNode.hpp"
#include "VariableDeclarationNode.hpp"
#include <context/VirtualTable.hpp>
#include <constants/AccessModifierType.hpp>


namespace LynxAst {

    using namespace LynxConstants;

    class InterfaceDeclarationNode : public Node {

        private:

            std::string interfaceName;

            llvm::StructType* llvmStructType = nullptr;

            llvm::PointerType* llvmPointerType = nullptr;

            std::vector<std::string> inheritIfaces;

            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>> methods;

            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> fields;

        private:

            void processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members);

            void attachFunctionSignature(llvm::Function* fn, const std::string& signature);

            void buildInterfaceType(const AstContext& astContext, InterfaceType& ifaceType);

            void emitBaseInterfaces(const AstContext& astContext, const GlobalSymbolContext& symbol, VirtualTable& vtable);

            void emitMethods(const AstContext& astContext, const InterfaceType& ifaceType, VirtualTable& vtable);

            llvm::Function* generateMemberFunction(const AstContext& astContext, const InterfaceType& ifaceType, FunctionNode* fnNode);

            void emitVTableGlobal(AstContext& astContext, std::unique_ptr<InterfaceType> ifaceType, VirtualTable& vtable);

        public:

            InterfaceDeclarationNode(std::string name) : interfaceName(name) {}

            explicit InterfaceDeclarationNode(
                std::string name, 
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : interfaceName(name) {
                processMembers(std::move(members));
            }

            explicit InterfaceDeclarationNode(
                std::string name, 
                std::vector<std::string> parents,
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : interfaceName(std::move(name)), inheritIfaces(std::move(parents)) {
                processMembers(std::move(members));
            }

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::INTERFACE_DECLARATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] inline bool hasParentInterfaces() const noexcept { return !inheritIfaces.empty(); }

            [[nodiscard]] inline const std::string& getInterfaceName() const noexcept { return interfaceName; }

            [[nodiscard]] inline const std::vector<std::string>& getParentInterfaceNames() const noexcept { return inheritIfaces; }

            [[nodiscard]] inline const std::string getQualifiedName() const noexcept { return Mangle::get(ManglerKind::CLASS, interfaceName); }

            ~InterfaceDeclarationNode() override = default;
    };

}

#endif
