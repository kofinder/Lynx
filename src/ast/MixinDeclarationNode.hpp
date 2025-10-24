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

            NodeType getNodeType() override { return NodeType::MIXIN_DECLARATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
            
            inline const std::string& getOriginalName() const { return mixinName; }

            inline bool hasParentMixins() const { return !inheritMixins.empty(); }

            inline llvm::StructType* getStructType() const { return llvmStructType; }

            inline llvm::PointerType* getPointerType() const { return llvmPointerType; }

            inline const std::string getQualifiedName() const { return Mangle::get(ManglerKind::CLASS, mixinName); }
            
            ~MixinDeclarationNode() override = default;

    };

}

#endif
