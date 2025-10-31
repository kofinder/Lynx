#ifndef LYNX_OBJECT_CREATION_NODE_HPP
#define LYNX_OBJECT_CREATION_NODE_HPP

#include "Node.hpp"
#include "ExpressionNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include "ClazzConstructorNode.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include <constants/NodeType.hpp>

using namespace LynxConstants;

namespace LynxAst {

    typedef struct { 
        std::vector<llvm::Type*> argTypes; 
        std::vector<llvm::Value*> argValues; 
        llvm::Type* objectType; 
        BaseType* baseType;
    } CallableInfo;

    class ObjectCreationNode : public Node {
        
        private:

            std::shared_ptr<VariableType> variableType;

            std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> arguments;

            CallableInfo resolveCallableLLVMInfo(AstContext& astContext);

            std::tuple<llvm::Type*, BaseType*> convertToLLVMType(AstContext& astContext);

            std::string getMangleName(const CallableInfo& callableInfo) const;
            
            llvm::Value* generate(AstContext& astContext, const CallableInfo& callableInfo);

            void emitConstructorCall(AstContext& astContext, llvm::Value* newInstance, const CallableInfo& callableInfo);

        public:

            ObjectCreationNode(
                std::shared_ptr<VariableType> varType, 
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : variableType(std::move(varType)), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override; 

            NodeType getNodeType() override { return NodeType::OBJECT_CREATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const { return *arguments; }
            
            ~ObjectCreationNode() override = default;
    };
}

#endif
