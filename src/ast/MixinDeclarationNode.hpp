/**
 * @file MixinDeclarationNode.hpp
 * @brief Declares the MixinDeclarationNode class, representing mixin declarations in the Lynx AST.
 * 
 * The MixinDeclarationNode class models user-defined mixins, including their fields, methods, and
 * inherited mixins. It supports semantic analysis, LLVM IR type generation, and method emission.
 * 
 * **Key Responsibilities:**
 * - Stores the mixin name, fields, methods, and parent mixins.
 * - Processes member nodes to separate methods and fields.
 * - Generates LLVM struct and pointer types for the mixin.
 * - Emits LLVM IR for member functions.
 * - Supports deep cloning of the node and its members.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for user-defined mixin types.
 * 
 * @see FunctionNode, VariableDeclarationNode, MixinType, Mangle
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_MIXIN_DECLARATION_NODE_HPP
#define LYNX_MIXIN_DECLARATION_NODE_HPP

#include "Node.hpp"
#include "FunctionNode.hpp"
#include <tmpl/ManglerTemplate.hpp>
#include "VariableDeclarationNode.hpp"
#include <context/GlobalSymbolContext.hpp>
#include <types/userdefined/MixinType.hpp>
#include <constants/AccessModifierType.hpp>
#include <constants/ClassInheritanceType.hpp>

namespace LynxAst {
    
    using namespace LynxConstants;
    using namespace LynxContext;

    class MixinDeclarationNode : public Node {

        private:

            std::string mixinName;

            llvm::StructType* llvmStructType = nullptr;

            llvm::PointerType* llvmPointerType = nullptr;

            std::vector<std::string> inheritMixins;

            std::unique_ptr<std::vector<std::unique_ptr<FunctionNode>>> methods;

            std::unique_ptr<std::vector<std::unique_ptr<VariableDeclarationNode>>> fields;
        
        private:

            void processMembers(std::unique_ptr<std::vector<std::unique_ptr<Node>>> members);

            void buildMixinType(const AstContext& astContext, MixinType& mixinType);

            void emitMethods(const AstContext& astContext, const MixinType& mixinType);

            llvm::Function* generateMemberFunction(const AstContext& astContext, const MixinType& mixinType, FunctionNode* fnNode);

        public:

            MixinDeclarationNode(
                std::string name
            ) : mixinName(std::move(name)) {}

            explicit MixinDeclarationNode(
                std::string name, 
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : mixinName(std::move(name)) {
                processMembers(std::move(members));
            }
        
            explicit MixinDeclarationNode(
                std::string name, 
                std::vector<std::string> parents,
                std::unique_ptr<std::vector<std::unique_ptr<Node>>> members
            ) : mixinName(std::move(name)), inheritMixins(std::move(parents)) {
                processMembers(std::move(members));
            }

            std::unique_ptr<Node> clone() const override;

            inline constexpr NodeType getNodeType() override { return NodeType::MIXIN_DECLARATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
            
            [[nodiscard]] inline const std::string& getOriginalName() const { return mixinName; }

            [[nodiscard]] inline bool hasParentMixins() const { return !inheritMixins.empty(); }

            [[nodiscard]] inline llvm::StructType* getStructType() const { return llvmStructType; }

            [[nodiscard]] inline llvm::PointerType* getPointerType() const { return llvmPointerType; }

            [[nodiscard]] inline const std::string getQualifiedName() const { return Mangle::get(ManglerKind::CLASS, mixinName); }
            
            ~MixinDeclarationNode() override = default;

    };

}

#endif
