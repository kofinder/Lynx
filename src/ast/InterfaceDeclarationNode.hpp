#ifndef LYNX_INTERFACE_DECLARATION_NODE_HPP
#define LYNX_INTERFACE_DECLARATION_NODE_HPP

#include "Node.hpp"
#include "FunctionNode.hpp"
#include <tmpl/ManglerTemplate.hpp>
#include "ClazzConstructorNode.hpp"
#include "VariableDeclarationNode.hpp"
#include <context/VirtualTable.hpp>
#include <constants/AccessModifierType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    class InterfaceDeclarationNode : public Node {

        private:

            std::string interfaceName;

            llvm::StructType* llvmStructType = nullptr;

            llvm::PointerType* llvmPointerType = nullptr;

            std::vector<std::string> inheritIfaces;

            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>> methods;

            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> fields;

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

            NodeType getNodeType() override { return NodeType::INTERFACE_DECLARATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            inline bool hasParentInterfaces() const { return !inheritIfaces.empty(); }

            inline const std::string& getInterfaceName() const { return interfaceName; }

            inline const std::vector<std::string>& getParentInterfaceNames() const { return inheritIfaces; }

            inline const std::string getQualifiedName() const { return Mangle::get(ManglerKind::CLASS, interfaceName); }

            ~InterfaceDeclarationNode() override = default;
    };

}

#endif
