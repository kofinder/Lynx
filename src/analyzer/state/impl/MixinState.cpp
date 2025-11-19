#include "state/MixinState.hpp"
#include <logger/Logger.hpp>
#include <ast/MixinDeclarationNode.hpp>
#include <interfaces/SemanticAnalyzer.hpp>
#include <types/userdefined/helper/MethodType.hpp>
#include <types/userdefined/helper/FieldType.hpp>

namespace LynxAnalyzer {

    using namespace LynxAst;
    using namespace LynxLogger;

    bool MixinState::detectCycle(MixinType* mixinType, std::unordered_map<std::string,int>& state, std::vector<std::string>& stack, std::string& errMsg) const {
        const std::string name = mixinType->qualifiedName();
        if (state[name] == 1) {
            // cycle detected
            errMsg = "Circular mixin detected: ";
            auto it = std::find(stack.begin(), stack.end(), name);
            for (; it != stack.end(); ++it) errMsg += *it + " -> ";
            errMsg += name;
            return false;
        }
        if (state[name] == 2) return true; // already processed

        state[name] = 1;
        stack.push_back(name);

        for (const auto* parent : mixinType->getParents()) {
            if (!detectCycle(const_cast<MixinType*>(parent), state, stack, errMsg)) return false;
        }

        stack.pop_back();
        state[name] = 2;
        return true;
    }

    bool MixinState::validateFlattenedMixin(MixinType* mixin, std::string& errMsg) const {
        std::cout << "validated falttned mixin" << std::endl;
        std::unordered_map<std::string, FieldType*> fieldNames;
        std::unordered_map<std::string, MethodType*> methodSigs;
    
        // merge parents
        for (const auto* parent : mixin->getParents()) {
            for (const auto& f : parent->getFlattenedFields()) {
                if (fieldNames.count(f->getName())) {
                    errMsg = "Field conflict in mixin '" + mixin->qualifiedName() + "': " + f->getName();
                    return false;
                }
                fieldNames[f->getName()] = f.get();
            }
            for (const auto& [n, m] : parent->getFlattenedMethods()) {
                if (methodSigs.count(m->getMethodSignature())) {
                    errMsg = "Method conflict in mixin '" + mixin->qualifiedName() + "': " + m->getMethodSignature();
                    return false;
                }
                methodSigs[m->getMethodSignature()] = m.get();
            }
        }
    
        // merge child’s own members
        for (const auto& [name, f] : mixin->getFields()) {
            if (fieldNames.count(f->getName())) {
                errMsg = "Field '" + f->getName() + "' in mixin '" + mixin->qualifiedName() + "' conflicts with parent field";
                return false;
            }
            fieldNames[f->getName()] = f.get();
        }
    
        for (const auto& [n, m] : mixin->getMethods()) {
            // override parent if signature matches
            methodSigs[m->getMethodSignature()] = m.get();
        }
    
        mixin->setFlattenedFields(fieldNames);   // store for later LLVM lowering
        mixin->setFlattenedMethods(methodSigs);
    
        return true;    
    }

    void MixinState::handleNode(SemanticAnalyzer& analyzer, Node* node) {
        auto* mixinNode = dynamic_cast<MixinDeclarationNode*>(node);
        if (!mixinNode) return;
    
        // MixinType* mixinType = mixinNode->getMixinType();
        MixinType* mixinType = mixinNode->getStructType() ? MixinType::fromLLVMType(mixinNode->getStructType()) : nullptr;
        if (!mixinType) {
            analyzer.reportError("MixinType not initialized", mixinNode->getLineNumber());
            return;
        }
    
        std::string errMsg;
        std::unordered_map<std::string, int> dfsState;
        std::vector<std::string> stack;
    
        if (!detectCycle(mixinType, dfsState, stack, errMsg)) {
            analyzer.reportError(errMsg, node->getLineNumber());
            return;
        }
    
        if (!validateFlattenedMixin(mixinType, errMsg)) {
            analyzer.reportError(errMsg, node->getLineNumber());
            return;
        }    
    }

}