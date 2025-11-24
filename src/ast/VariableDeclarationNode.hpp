/**
 * @file VariableDeclarationNode.hpp
 * @brief Declares the VariableDeclarationNode class, representing variable declarations in the AST.
 * 
 * VariableDeclarationNode models the declaration of local, global, and class member variables
 * in the Lynx language. It supports optional initializers, type annotations, access modifiers,
 * and tracks LLVM references for code generation.
 * 
 * **Key Responsibilities:**
 * - Represent variable declarations with optional type and initializer.
 * - Distinguish between class variables, local variables, and global variables.
 * - Support mutable and captured-in-closure variables.
 * - Provide LLVM IR code generation for all variable types.
 * - Calculate array/collection size for multi-dimensional declarations.
 * 
 * **Used By:**
 * - Semantic analyzer for type and scope checking.
 * - IR generator for LLVM code emission.
 * - Compiler passes that handle variable resolution and assignment.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_VARIABLE_DECLARATION_NODE_HPP
#define LYNX_VARIABLE_DECLARATION_NODE_HPP

#include "Node.hpp"
#include <types/interfaces/BaseType.hpp>
#include <constants/VariableType.hpp>
#include <constants/DataType.hpp>
#include <constants/AccessModifierType.hpp>
#include <constants/VariableDeclarationType.hpp>

namespace LynxAst {

    using namespace LynxTypes;
    using namespace LynxConstants;

    class StatementListNode;

    class VariableDeclarationNode : public Node {

        protected:
        
            std::string variableName;
            std::unique_ptr<Node> initialValueNode;
            std::unique_ptr<Node> clazzNode;
            std::unique_ptr<Node> functionNode;
            std::shared_ptr<VariableType> variableType;

            bool isMutable = false;
            bool isCapturedInClosure = false;
        
            llvm::Value* llvmVariableRef = nullptr;
            BaseType* baseType = nullptr;
                
        private:

            AccessModifierType accessModifier = AccessModifierType::DEFAULT;

            llvm::Value* generateGlobalVariable(const AstContext& astContext, StatementListNode& stmtNode);

            llvm::Value* generateLocalVariable(const AstContext& astContext, StatementListNode& stmtNode);

            llvm::Value* generateCollectionVariable(const AstContext& astContext, StatementListNode& stmtNode);

            llvm::Value* generateBuiltinTypeOrMemberVariable(const AstContext& astContext, StatementListNode& stmtNode);

            bool isAllocatedOnHeap(const AstContext& astContext) const;
        
        public:
            explicit VariableDeclarationNode(std::string varName) : variableName(varName) {}
        
            VariableDeclarationNode(
                std::shared_ptr<VariableType> varType, 
                std::string varName
            ): variableName(varName), variableType(std::move(varType)) {}
        
            VariableDeclarationNode(
                BaseType* baseType, 
                std::string varName
            ): variableName(varName), baseType(baseType) {}
        
            VariableDeclarationNode(
                BaseType* baseType, 
                std::string varName, 
                std::unique_ptr<Node> initializerNode
            ): variableName(varName), baseType(baseType), isMutable(true), initialValueNode(std::move(initializerNode)) {}
        
            VariableDeclarationNode(
                std::shared_ptr<VariableType> varType, 
                std::string varName, 
                std::unique_ptr<Node> initializerNode
            ): variableName(varName), variableType(varType), isMutable(true), initialValueNode(std::move(initializerNode)) {}
        
            std::unique_ptr<Node> clone() const override;
        
            inline constexpr NodeType getNodeType() override { return NodeType::VARIABLE_DECLARATION_NODE; }
        
            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;
                            
            void setInitializer(std::unique_ptr<Node> node) { initialValueNode = std::move(node); }

            void setClazzNode(std::unique_ptr<Node> clazz) { clazzNode = std::move(clazz); }
        
            [[nodiscard]] bool isClassVariable() const { return clazzNode != nullptr; }
        
            [[nodiscard]] llvm::Value* getLLVMVariableRef() const { return llvmVariableRef; }
            void setLLVMVariableRef(llvm::Value* val) { llvmVariableRef = val; }
        
            [[nodiscard]] BaseType* getBaseType() const { return baseType; }
            [[nodiscard]] const std::string& getVariableName() const { return variableName; }
            [[nodiscard]] VariableType* getVariableType() const { return variableType.get(); }
        
            [[nodiscard]] bool hasInitializer() const { return initialValueNode != nullptr; }
            [[nodiscard]] Node* getInitializer() const { return initialValueNode.get(); }
        
            void setAccessModifier(AccessModifierType modifier) { accessModifier = modifier; }
            [[nodiscard]] inline constexpr AccessModifierType getAccessModifier() const { return accessModifier; }
        
            size_t calculateDeclaredSize(const std::vector<int>& dimensions, size_t index = 0) const noexcept {
                if (index >= dimensions.size()) return 1;
                return dimensions[index] * calculateDeclaredSize(dimensions, index + 1);
            }

            ~VariableDeclarationNode() override = default;
    };  

}

#endif
