#include "FunctionNode.hpp"
#include "ClazzDeclarationNode.hpp"
#include "ArrayAccessNode.hpp"
#include "IdentifierNode.hpp"
#include "utils/VariableUtils.hpp"
#include "tmpl/CloneNodeTemplate.hpp"
#include "AssignmentExpressionNode.hpp"
#include "VariableDeclarationNode.hpp"

namespace LynxAst {

    using namespace VariableUtils;
    using namespace Cloneable;

    llvm::Value* AssignmentExpressionNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation .....");
        if(assignExprType == AssignExpressionType::SIMPLE_ASSIGN) {
           return generateSimpleAssign(*astContext);
        } else {
            return generateComplexAssign(*astContext);
        }
    }

    llvm::Value* AssignmentExpressionNode::generateComplexAssign(const AstContext& astContext) {

        auto resolved = resolveVariable(astContext, varName);
        if (!resolved.value || !resolved.reference) {
            LOG_ERROR("Failed to resolve variable '{}'", varName);
            throw std::runtime_error("Variable resolution failed.");
        }

        llvm::Type* valueType = resolved.value->getType();
        llvm::Value* llvmVarRef = resolved.reference;
        llvm::Value* newValue = nullptr;


        auto& builder = astContext.getBuilder();
        auto* lhsValue = builder.CreateLoad(valueType, llvmVarRef);
        auto* rhsValue = expressionNode->generateCode(astContext.createContext());

        switch (operatorType) {
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
    
    llvm::Value* AssignmentExpressionNode::generateSimpleAssign(const AstContext& astContext) {
        auto& builder = astContext.getBuilder();
        auto* lhsPtr = assignableNode->generateCode(astContext.createContext());
        auto* rhsValue = expressionNode->generateCode(astContext.createContext());
        if (!lhsPtr || !rhsValue) {
            LOG_ERROR("Null in assignment operands.");
            return nullptr;
        }
        
        return builder.CreateStore(rhsValue, lhsPtr);
    }

    std::unique_ptr<Node> AssignmentExpressionNode::clone() const {
        auto clonedExpr = cloneNode(expressionNode);
        if (assignableNode) {
            return std::make_unique<AssignmentExpressionNode>(Cloneable::cloneNode(assignableNode), operatorType, std::move(clonedExpr), assignExprType);
        } else if (!baseName.empty()) {
            return std::make_unique<AssignmentExpressionNode>(varName, baseName, operatorType, std::move(clonedExpr), assignExprType);
        } else {
            return std::make_unique<AssignmentExpressionNode>(const_cast<char*>(varName.c_str()), operatorType, std::move(clonedExpr),assignExprType);
        }    
    }
    
}
