#include "FunctionNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include "ArrayAccessNode.hpp"
#include "IdentifierNode.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "AssignmentExpressionNode.hpp"
#include "VariableDeclarationNode.hpp"

namespace LynxAst {

    llvm::Value* AssignmentExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        if(assignExprType == AssignExpressionType::SIMPLE_ASSIGN) {
           return generateSimpleAssign(astContext);
        } else {
            return generateComplexAssign(astContext);
        }
    }

    llvm::Value* AssignmentExpressionNode::generateComplexAssign(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Dispatched ....");

        auto synbol = astContext->getGlobalContext();
        Node* varNode = synbol->findVariable(this->varName);

        if(varNode == nullptr) {
            LOG_ERROR("Invalid variable name: {}", this->varName);
            throw std::runtime_error("Invalid variable name");
        }

        auto* variable = dynamic_cast<VariableDeclarationNode*>(varNode);
        if (!variable) {
            LOG_ERROR("Variable '{}' is not of type VariableDeclarationNode.", this->varName);
            throw std::runtime_error("Invalid variable type for unary operation.");
        }

        llvm::Value* llvmVarRef = variable->getLLVMVariableRef();
        if (!llvmVarRef || !llvmVarRef->getType()->isPointerTy()) {
            LOG_ERROR("Variable '{}' is not a valid pointer.", this->varName);
            throw std::runtime_error("Invalid LLVM variable reference.");
        }

        llvm::Type* loadType = llvmVarRef->getType()->getPointerElementType();
        if (!loadType) {
            LOG_ERROR("Failed to determine pointer element type.");
            throw std::runtime_error("Failed to determine pointer element type.");
        }
        
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();

        llvm::Value* lhsValue = builder.CreateLoad(loadType, llvmVarRef);
        llvm::Value* rhsValue = this->expressionNode->generateCode(astContext->createContext());
        llvm::Value* newValue = nullptr;

        switch (this->operatorType) {
            case PLUS_ASSIGN:
                newValue = builder.CreateAdd(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignAdd));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case MINUS_ASSIGN:
                newValue = builder.CreateSub(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignMinus));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case MULTIPLY_ASSIGN:
                newValue = builder.CreateMul(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignMultiply));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case DIVIDE_ASSIGN:
                newValue = builder.CreateSDiv(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignDivide));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case MODULO_ASSIGN:
                newValue = builder.CreateSRem(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignModulo));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case LSHIFT_ASSIGN:
                newValue = builder.CreateShl(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignLShift));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case RSHIFT_ASSIGN:
                newValue = builder.CreateAShr(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignRShift));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case AND_ASSIGN:
                newValue = builder.CreateAnd(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignAnd));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            case XOR_ASSIGN:
                newValue = builder.CreateXor(lhsValue, rhsValue, llvm::Twine(LynxLabelTypeConstants::assignXor));
                builder.CreateStore(newValue, llvmVarRef);
                break;
            default:
                LOG_ERROR("Unsupported assignment operator.");
            return nullptr;
        }

        return newValue;
    }
    
    llvm::Value* AssignmentExpressionNode::generateSimpleAssign(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Invoked Simple Assign....");
        auto* module = astContext->getModule();
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        
        llvm::Value* lhsPtr = assignableNode->generateCode(astContext->createContext());
        llvm::Value* rhsValue = expressionNode->generateCode(astContext->createContext());

        if (!lhsPtr || !rhsValue) {
            LOG_ERROR("Null in assignment operands.");
            return nullptr;
        }
        
        return builder.CreateStore(rhsValue, lhsPtr);
    }

    std::unique_ptr<Node> AssignmentExpressionNode::clone() const {
        auto clonedExpr = Cloneable::cloneNode(expressionNode);
        if (assignableNode) {
            return std::make_unique<AssignmentExpressionNode>(Cloneable::cloneNode(assignableNode), operatorType, std::move(clonedExpr), assignExprType);
        } else if (!baseName.empty()) {
            return std::make_unique<AssignmentExpressionNode>(varName, baseName, operatorType, std::move(clonedExpr), assignExprType);
        } else {
            return std::make_unique<AssignmentExpressionNode>(const_cast<char*>(varName.c_str()), operatorType, std::move(clonedExpr),assignExprType);
        }    
    }
    
}
