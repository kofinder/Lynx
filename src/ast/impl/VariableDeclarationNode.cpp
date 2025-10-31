#include <unordered_set>
#include <logger/Logger.hpp>
#include "StatementListNode.hpp" 
#include "PrimaryExpressionNode.hpp"
#include "VariableDeclarationNode.hpp"
#include "CollectionInitializerNode.hpp"
#include <constants/LinkageType.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include "utils/CollectionUtils.hpp"
#include "ObjectCreationNode.hpp"
#include <exceptions/impl/SemanticException.cpp>

namespace LynxAst {

    using namespace LynxExceptions;
    using namespace LynxLogger;
    using namespace LynxContext;
    
    llvm::Value* VariableDeclarationNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Executed...... {}", variableName);

        auto symbol = astContext->getGlobalContext();
        auto stmtNode = symbol->getStatementNode();

        auto* currentBlock = dynamic_cast<StatementListNode*>(stmtNode);

        if (currentBlock != nullptr && currentBlock->findLocal(this->variableName) != nullptr) {
            LOG_ERROR("variable with name : {} already exists in this block", variableName);
            throw new SemanticException("variable with name : '" + variableName + "' already exists in this block", lineNumber);
        }
        
        if(auto funcNode = symbol->getFunctionNode()) {
            return generateLocalVariable(*astContext, *currentBlock);
        }
        
        return generateGlobalVariable(*astContext, *currentBlock);
    }

    llvm::Value* VariableDeclarationNode::generateGlobalVariable(const AstContext& astContext, StatementListNode& stmtNode) {
        LOG_INFO("Executed...... {}", variableName);

        auto* value = initialValueNode->generateCode(astContext.createContext());

        auto* module = astContext.getModule();
        llvmVariableRef = new llvm::GlobalVariable(*module, value->getType(), false, internalType, 0, variableName);

        static_cast<llvm::GlobalVariable*>(llvmVariableRef)->setInitializer(dyn_cast<llvm::Constant>(value));

        stmtNode.createLocal(variableName, this);

        return llvmVariableRef;
    }

    llvm::Value* VariableDeclarationNode::generateLocalVariable(const AstContext& astContext, StatementListNode& stmtNode) {
        static const std::unordered_set<DataType> collectionTypes = {
            DataType::ARRAY,
            DataType::VECTOR,
            DataType::LIST,
            DataType::SET,
            DataType::MAP,
            DataType::STACK,
            DataType::QUEUE,
            DataType::TREE,
            DataType::GRAPH,
            DataType::HEAP
        };
    
        if (collectionTypes.count(variableType->type)) {
            return generateCollectionVariable(astContext, stmtNode);
        }
    
        return generateBuiltinTypeOrMemberVariable(astContext, stmtNode);    
    }

    llvm::Value* VariableDeclarationNode::generateBuiltinTypeOrMemberVariable(const AstContext& astContext, StatementListNode& stmtNode) {
        LOG_INFO("Executed...... {}", variableName);

        auto& builder = astContext.getBuilder();

        stmtNode.createLocal(variableName, this);
        llvm::Value* initializerValue = nullptr;
        if(initialValueNode) {
            initializerValue = initialValueNode->generateCode(astContext.createContext());
            if (llvm::isa<llvm::ConstantPointerNull>(initializerValue)) {
                llvm::errs() << "This constant is zero-initialized / null!\n";
                auto declaredType = astContext.findType(variableType->type);
                llvmVariableRef = declaredType->createInstance(variableName);
                declaredType->assignTo(llvmVariableRef, declaredType->getDefaultValue());
                return llvmVariableRef;
            }

            const auto inferredType = astContext.findType(initializerValue);
            if (!inferredType) return nullptr;
            if (auto* autoType = TypeCasting::castType<AutoType>(inferredType.get())) {
                const_cast<AutoType*>(autoType)->setInferredType(inferredType.get());
                baseType = autoType;
            } else {
                baseType = inferredType.get();
            }    
        } 

        // --- Fallback to explicitly declared type ---
        if (!baseType) {
            const auto declaredType = astContext.findType(variableType->type);
            if (!declaredType) {
                LOG_ERROR("Declared type not found for '{}'", variableName);
                return nullptr;
            }

            if (auto* autoType = TypeCasting::castType<AutoType>(declaredType.get())) {
                const_cast<AutoType*>(autoType)->setInferredType(declaredType.get());
                baseType = autoType;
            } else {
                baseType = declaredType.get();
            }
        }
    
        if(isAllocatedOnHeap(astContext)) {
            llvmVariableRef = initializerValue;
        } else {
            llvmVariableRef = baseType->createInstance(variableName);
        }

        if (initializerValue && !isAllocatedOnHeap(astContext)) {
            baseType->assignTo(llvmVariableRef, initializerValue);
        }

        return llvmVariableRef;
    }


    llvm::Value* VariableDeclarationNode::generateCollectionVariable(const AstContext& astContext, StatementListNode& stmtNode) {
        LOG_INFO("Executed...... {}", variableName);

        auto& builder = astContext.getBuilder();
        auto& context = astContext.getLLVMContext();
        auto* module = astContext.getModule();

        auto* value = initialValueNode->generateCode(astContext.createContext());

        auto baseType = astContext.findType(value);
        llvmVariableRef = baseType->createInstance(variableName);
       
        std::cout << "yes" << toString(baseType->getTypeTag())<< std::endl;

        llvm::Type* llvmType = baseType->getLLVMType();
        if (!llvmType->isSized()) {
            llvm::errs() << "ERROR: LLVM type is unsized:\n";
            llvmType->print(llvm::errs());
            llvm::errs() << "\n";
            llvm::report_fatal_error("Unsized type in codegen");
        }
        

        auto* constVal = llvm::cast<llvm::Constant>(value);
        auto* globalConstant = new llvm::GlobalVariable(*module, llvmType, true, privateType, constVal, "const_array");
        globalConstant->setAlignment(llvm::Align(4));

        auto* destPtr = builder.CreateBitCast(llvmVariableRef, llvm::Type::getInt8PtrTy(context), llvm::Twine(variableName + "_" + "bitcast"));
        auto* srcPtr = builder.CreateBitCast(globalConstant, llvm::Type::getInt8PtrTy(context));

        llvm::DataLayout dataLayout(module);
        uint64_t allocSize = dataLayout.getTypeAllocSize(llvmType);
        llvm::Value* totalSize = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), allocSize);
        llvm::Value* isVolatile = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), false);

        auto* memFunc = llvm::Intrinsic::getDeclaration(module, llvm::Intrinsic::memcpy, {
            llvm::Type::getInt8PtrTy(context),  // Destination pointer (i8*)
            llvm::Type::getInt8PtrTy(context),  // Source pointer (i8*)
            llvm::Type::getInt64Ty(context),    // Number of bytes to copy (i64)
        });

        builder.CreateCall(memFunc, {destPtr, srcPtr, totalSize, isVolatile});

        return llvmVariableRef;
    }    

    bool VariableDeclarationNode::isAllocatedOnHeap(const AstContext& astContext) const {
        if (auto exprNode = dynamic_cast<ExpressionNode*>(initialValueNode.get())) {
            if (auto primaryExpNode = dynamic_cast<PrimaryExpressionNode*>(exprNode->expressionNode.get())) {
                if (auto objCreation = dynamic_cast<ObjectCreationNode*>(primaryExpNode->getInnerExpression())) {
                    return true;
                }
            }
        }
        return false;
    }

    std::unique_ptr<Node> VariableDeclarationNode::clone() const {
        auto copy = std::make_unique<VariableDeclarationNode>(variableType, variableName);

        if (initialValueNode) {        
            auto cloned = initialValueNode->clone();
            if (!cloned) {
                std::cerr << "Clone returned nullptr!" << std::endl;
            }
            copy->setInitializer(std::move(cloned));
        }
    
        if (clazzNode) {
            copy->setClazzNode(clazzNode->clone());
        }
    
        copy->setAccessModifier(accessModifier);
        copy->baseType = baseType;

        return copy;
    }
    
}
