/**
 * @file ObjectCreationNode.hpp
 * @brief Declares the ObjectCreationNode class, representing object instantiation in the Lynx AST.
 * 
 * The ObjectCreationNode class models the creation of objects, including constructor argument evaluation,
 * LLVM IR type conversion, and constructor calls. It maintains the variable type and a list of arguments
 * used during object instantiation.
 * 
 * **Key Responsibilities:**
 * - Stores the variable type of the object being created.
 * - Maintains a list of constructor arguments.
 * - Resolves LLVM types and callable information for constructor calls.
 * - Supports LLVM IR generation for object allocation and constructor invocation.
 * - Provides deep cloning of the node and its arguments.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR code generation for object creation expressions.
 * 
 * @see ExpressionNode, VariableType, ClazzDeclarationNode, ClazzConstructorNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_OBJECT_CREATION_NODE_HPP
#define LYNX_OBJECT_CREATION_NODE_HPP

#include "Node.hpp"
#include "ExpressionNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include "ClazzConstructorNode.hpp"
#include "tmpl/ManglerTemplate.hpp"
#include <constants/NodeType.hpp>

namespace LynxAst {

    using namespace LynxConstants;

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

        private:

            CallableInfo resolveCallableLLVMInfo(const AstContext& astContext);

            std::tuple<llvm::Type*, BaseType*> convertToLLVMType(const AstContext& astContext);

            std::string getMangleName(const CallableInfo& callableInfo) const;
            
            llvm::Value* generateWithGC(const AstContext& astContext, const CallableInfo& callableInfo);

            void emitConstructorCall(const AstContext& astContext, llvm::Value* newInstance, const CallableInfo& callableInfo);

        public:

            ObjectCreationNode(
                std::shared_ptr<VariableType> varType, 
                std::unique_ptr<std::vector<std::unique_ptr<ExpressionNode>>> args
            ) : variableType(std::move(varType)), arguments(std::move(args)) {}

            std::unique_ptr<Node> clone() const override; 

            inline constexpr NodeType getNodeType() override { return NodeType::OBJECT_CREATION_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            [[nodiscard]] const std::vector<std::unique_ptr<ExpressionNode>>& getArguments() const { return *arguments; }
            
            ~ObjectCreationNode() override = default;
    };
}

#endif
