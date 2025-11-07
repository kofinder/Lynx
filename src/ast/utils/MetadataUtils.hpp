/**
 * @file MetadataUtils.hpp
 * @brief Utilities for attaching and retrieving LLVM metadata on functions and global variables.
 *
 * Provides helper functions to attach string metadata, retrieve metadata values, and
 * create composite metadata nodes for fields or other structured information.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_METADATA_UTILS
#define LYNX_METADATA_UTILS

#include <string>
#include <vector>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"

namespace LynxAst::metadata {

    class Utils {
        
        public:
            /// Attach a string metadata to a function
            static void attach(llvm::Function* fn, const std::string& key, const std::string& value) {
                if (!fn) return;
                llvm::LLVMContext &ctx = fn->getContext();

                llvm::MDString *mdKey = llvm::MDString::get(ctx, key);
                llvm::MDString *mdValue = llvm::MDString::get(ctx, value);

                llvm::MDNode *node = llvm::MDNode::get(ctx, { mdKey, mdValue });
                fn->setMetadata(key, node);
            }

            /// Retrieve a string metadata value from a function
            static std::string get(llvm::Function* fn, const std::string& key) {
                if (!fn) return "";
                if (auto* node = fn->getMetadata(key)) {
                    if (node->getNumOperands() >= 2) {
                        if (auto* mds = llvm::dyn_cast<llvm::MDString>(node->getOperand(1))) {
                            return mds->getString().str();
                        }
                    }
                }
                return "";
            }

            /// Attach metadata to a global variable
            static void attach(llvm::GlobalVariable* gv, const std::string& key, const std::string& value) {
                if (!gv) return;
                llvm::LLVMContext &ctx = gv->getContext();

                llvm::MDString *mdKey = llvm::MDString::get(ctx, key);
                llvm::MDString *mdValue = llvm::MDString::get(ctx, value);
                llvm::MDNode *node = llvm::MDNode::get(ctx, { mdKey, mdValue });

                gv->addMetadata(0, *node);
            }

            /// Create a composite metadata node (e.g. for fields)
            static llvm::MDNode* createNode(llvm::LLVMContext& ctx, const std::vector<std::string>& values) {
                std::vector<llvm::Metadata*> mds;
                mds.reserve(values.size());
                for (const auto& v : values) {
                    mds.push_back(llvm::MDString::get(ctx, v));
                }
                return llvm::MDNode::get(ctx, mds);
            }

            /// Add a metadata node into a named metadata in a module
            static void addToNamedMetadata(llvm::Module* module, const std::string& name, llvm::MDNode* node) {
                if (!module || !node) return;
                llvm::NamedMDNode* named = module->getOrInsertNamedMetadata(name);
                named->addOperand(node);
            }
    };

}

#endif
