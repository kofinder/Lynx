#include <logger/Logger.hpp>

#include <constants/metadata/LabelTypeConstants.hpp>
#include "SwitchStatementNode.hpp"

using namespace LynxLogger;
using namespace LynxContext;
using namespace LynxLabelTypeConstants;

namespace LynxAst {
    
    llvm::Value* SwitchStatementNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("Procced ..............");
        
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        auto* module = astContext->getModule();


        llvm::Function* function = builder.GetInsertBlock()->getParent();
        llvm::Value* switchValue = levelExpression->generateCode(astContext);
    
        llvm::BasicBlock* defaultBB = llvm::BasicBlock::Create(context, lynxSwitchDefaultBB, function);
        llvm::BasicBlock* endBB = llvm::BasicBlock::Create(context, lynxSwitchExitBB, function);
        llvm::SwitchInst* switchInst = builder.CreateSwitch(switchValue, defaultBB, basicBlocks->size());

        llvm::BasicBlock* previousCaseBB = nullptr;

        for (auto& block : *basicBlocks) {
            auto* caseBlock = static_cast<SwitchCaseBlockNode*>(block.get());

            if (caseBlock->getCaseType() == SwitchBlockType::CASE) {
                if (caseBlock->isSkipCase()) {
                    continue;
                }

                llvm::BasicBlock* caseBB = llvm::BasicBlock::Create(context, lynxSwitchCaseBB, function);
                llvm::Value* caseValue = caseBlock->getExpressionNode()->generateCode(astContext->createContext());
                builder.SetInsertPoint(caseBB);

                if (caseBlock->getStatementNode() != nullptr) {
                    caseBlock->getStatementNode()->generateCode(astContext->createContext());
                }

                if (caseBlock->isFallThrough()) {
                    builder.CreateBr(previousCaseBB ? previousCaseBB : defaultBB);
                } else {
                    builder.CreateBr(endBB);
                }

                llvm::errs() << "switchValue ===>"; switchValue->print(llvm::outs()); llvm::errs() << "\n";
                llvm::errs() << "caseValue ===>"; caseValue->print(llvm::outs()); llvm::errs() << "\n";


                llvm::ConstantInt* caseConstant = tryGetCaseConstant(caseValue);
                if (caseConstant) {
                    llvm::Type* switchCondType = switchInst->getCondition()->getType();
                    if (caseConstant->getType() != switchCondType) {
                        LOG_ERROR("Case value does not match switch type......");
                        auto constExpr = llvm::ConstantExpr::getTruncOrBitCast(caseConstant, switchCondType);
                        caseConstant = llvm::cast<llvm::ConstantInt>(constExpr);
                    }
                    switchInst->addCase(caseConstant, caseBB);
                } else {
                    LOG_ERROR("Case value must be an integer constant.");
                }

                previousCaseBB = caseBB;
            } else {
                builder.SetInsertPoint(defaultBB);

                if (caseBlock->getStatementNode() != nullptr) {
                    caseBlock->getStatementNode()->generateCode(astContext->createContext());
                }

                builder.CreateBr(endBB);
                switchInst->setDefaultDest(defaultBB);
            }
        }

        bool hasDefaultBB = hasDefaultBlock();
        if (basicBlocks->empty() || !hasDefaultBB) {
            builder.SetInsertPoint(defaultBB);
            builder.CreateBr(endBB);
            switchInst->setDefaultDest(defaultBB);
        }

        builder.SetInsertPoint(endBB);

        return switchInst;
    }

    llvm::ConstantInt* SwitchStatementNode::tryGetCaseConstant(llvm::Value* value) {
        if (!value) return nullptr;
        
        // Direct constant int
        if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(value)) {
            return constInt;
        }

        // Struct with one i8 field (e.g., struct { i8 })
        if (auto* constStruct = llvm::dyn_cast<llvm::ConstantStruct>(value)) {
            if (constStruct->getNumOperands() > 0) {
                if (auto* innerConst = llvm::dyn_cast<llvm::ConstantInt>(constStruct->getOperand(0))) {
                    return innerConst; // Fixed: returning a ConstantInt*, not uint64_t
                }
            }
        }

        
        if (auto* constExpr = llvm::dyn_cast<llvm::ConstantExpr>(value)) {
            if (constExpr->getOpcode() == llvm::Instruction::ExtractValue) {
                llvm::Value* structVal = constExpr->getOperand(0);
                if (auto* constStruct = llvm::dyn_cast<llvm::ConstantStruct>(structVal)) {
                    if (constStruct->getNumOperands() > 0) {
                        if (auto* element = llvm::dyn_cast<llvm::ConstantInt>(constStruct->getOperand(0))) {
                            return element;
                        }
                    }
                }
            }
        }
    
        return nullptr;
    }
    

    bool SwitchStatementNode::hasDefaultBlock() const {
        return std::any_of(basicBlocks->begin(), basicBlocks->end(), [](auto& block) {
            return block->getCaseType() == SwitchBlockType::DEFAULT;
        });
        return false;
    }

    std::unique_ptr<Node> SwitchStatementNode::clone() const {
        auto clonedSwitch = std::make_unique<SwitchStatementNode>(std::move(levelExpression ? levelExpression->clone() : nullptr));

        if (basicBlocks) {
            for (const auto& block : *basicBlocks) {
                clonedSwitch->pushBasicBlock(block ? block->clone() : nullptr);
            }
        }

        return clonedSwitch;
    }
}
