/**
 * @file FunctionAttributeBuilder.hpp
 * @brief Utility class for constructing and applying LLVM function attributes.
 * 
 * The `FunctionAttributeBuilder` class provides a fluent interface for configuring
 * LLVM function attributes at various levels (function, return value, and parameters).
 * It is used to apply optimization hints and constraints that guide LLVM’s code generation
 * and optimization passes.
 * 
 * **Responsibilities:**
 * - Collect and manage function attributes before applying them.
 * - Support function-level, return-level, and parameter-level attribute application.
 * - Provide a chainable builder API for readability and convenience.
 * 
 * @see llvm::Function, llvm::AttrBuilder, llvm::AttributeList
 * @namespace LynxFunctionAttr
 * Provides helper classes and utilities for managing LLVM function attributes in Lynx.
 * 
 * @note This class simplifies LLVM attribute management, ensuring consistency
 *       across code generation stages within the Lynx compiler.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_FUNCTION_ATTRS_BUILDER_HPP
#define LYNX_FUNCTION_ATTRS_BUILDER_HPP

#include "llvm/IR/Function.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <vector>

namespace LynxFunctionAttr {

    class FunctionAttributeBuilder {

        private:

            llvm::Function* function;
            std::unique_ptr<llvm::AttrBuilder> fnAttrs;
            std::unique_ptr<llvm::AttrBuilder> retAttrs;
            std::vector<std::string> fnStringAttrs;
            std::vector<std::unique_ptr<llvm::AttrBuilder>> paramAttrs;
    
        
        public:

            explicit FunctionAttributeBuilder(llvm::Function* func)
            : function(func), 
            fnAttrs(std::make_unique<llvm::AttrBuilder>(func->getContext())),
            retAttrs(std::make_unique<llvm::AttrBuilder>(func->getContext())) {
                if (func) {
                    paramAttrs.reserve(func->arg_size());
                    for (unsigned i = 0; i < func->arg_size(); ++i) {
                        paramAttrs.push_back(std::make_unique<llvm::AttrBuilder>(func->getContext()));
                    }
                }
            }

            // Add enum-based function attribute
            FunctionAttributeBuilder& addAttribute(const llvm::Attribute& attr) {
                fnAttrs->addAttribute(attr);
                return *this;        
            }
    
            // Add enum-based return attribute
            FunctionAttributeBuilder& addAttributeAtRet(const llvm::Attribute& attr) {
                retAttrs->addAttribute(attr);
                return *this;        
            }
    
            // Add enum-based parameter attribute
            FunctionAttributeBuilder& addAttributeAtParam(const llvm::Attribute& attr, unsigned paramIndex) {
                if (paramIndex < paramAttrs.size()) {
                    paramAttrs[paramIndex]->addAttribute(attr);
                }
                return *this;        
            }
    
            // --- New method: add string attribute at function level ---
            FunctionAttributeBuilder& addStringAttribute(const std::string& attrName) {
                fnStringAttrs.push_back(attrName);
                return *this;
            }

            // --- New method: add string attribute at function level ---
            FunctionAttributeBuilder& addStringAttributeAtParam(const std::string &name, unsigned paramIndex) {
                if (paramIndex < paramAttrs.size()) {
                    paramAttrs[paramIndex]->addAttribute(llvm::Attribute::get(function->getContext(), name));
                }
                return *this;
            }            
    
            // Apply all accumulated attributes to the function
            void apply() {

                if (!function) return;

                auto &ctx = function->getContext();
                auto oldAL = function->getAttributes();
        
                // Function-level attributes
                if (fnAttrs && fnAttrs->hasAttributes()) {
                    auto fnAttrSet = llvm::AttributeSet::get(ctx, *fnAttrs);
                    auto newAL = oldAL.addFnAttributes(ctx, *fnAttrs);
                    function->setAttributes(newAL);
                    oldAL = newAL;
                }
        
                // Return value attributes
                if (retAttrs && retAttrs->hasAttributes()) {
                    auto newAL = oldAL.addRetAttributes(ctx, *retAttrs);
                    function->setAttributes(newAL);
                    oldAL = newAL;
                }
        
                // Parameter attributes
                for (unsigned i = 0; i < paramAttrs.size(); ++i) {
                    if (paramAttrs[i] && paramAttrs[i]->hasAttributes()) {
                        auto newAL = oldAL.addParamAttributes(ctx, i, *paramAttrs[i]);
                        function->setAttributes(newAL);
                        oldAL = newAL;
                    }
                }
        
                // String attributes (e.g., sanitizers)
                for (auto &sattr : fnStringAttrs) {
                    function->addFnAttr(llvm::Attribute::get(ctx, sattr));
                }

            }
    };

}

#endif 
