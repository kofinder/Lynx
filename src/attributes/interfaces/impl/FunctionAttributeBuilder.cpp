#include "interfaces/FunctionAttributeBuilder.hpp"
#include <llvm/IR/Attributes.h>

namespace LynxFunctionAttr {

    FunctionAttributeBuilder::FunctionAttributeBuilder(llvm::Function* func)
    : function(func),
      fnAttrs(func->getContext()),
      retAttrs(func->getContext()) {
        paramAttrs.reserve(func->arg_size());
        for (unsigned i = 0; i < func->arg_size(); ++i) {
            paramAttrs.emplace_back(func->getContext());
        }
    }

    FunctionAttributeBuilder& FunctionAttributeBuilder::addAttribute(llvm::Attribute::AttrKind attr) {
        fnAttrs.addAttribute(attr);
        return *this;
    }

    FunctionAttributeBuilder& FunctionAttributeBuilder::addAttributeAtRet(llvm::Attribute::AttrKind attr) {
        if (attr == llvm::Attribute::SExt) {
            retAttrs.removeAttribute(llvm::Attribute::ZExt);
        } else if (attr == llvm::Attribute::ZExt) {
            retAttrs.removeAttribute(llvm::Attribute::SExt);
        }
        retAttrs.addAttribute(attr);
        return *this;
    }

    FunctionAttributeBuilder& FunctionAttributeBuilder::addAttributeAtParam(llvm::Attribute::AttrKind attr, unsigned paramIndex) {
        if (paramIndex < paramAttrs.size()) {
            paramAttrs[paramIndex].addAttribute(attr);
        }
        return *this;
    }

    void FunctionAttributeBuilder::apply() {

        llvm::LLVMContext& ctx = function->getContext();

        std::vector<std::pair<unsigned, llvm::AttributeSet>> attrListPairs;
    
        // Return attributes (index = 0)
        if (retAttrs.hasAttributes()) { // <-- fix here
            attrListPairs.emplace_back(llvm::AttributeList::ReturnIndex, llvm::AttributeSet::get(ctx, retAttrs));
        }
    
        // Parameter attributes (indices = 1 to N)
        for (unsigned i = 0; i < paramAttrs.size(); ++i) {
            if (paramAttrs[i].hasAttributes()) { // <-- fix here
                attrListPairs.emplace_back(i + 1, llvm::AttributeSet::get(ctx, paramAttrs[i]));
            }
        }
    
        // Function attributes (index = ~0u)
        if (fnAttrs.hasAttributes()) { // <-- fix here
            attrListPairs.emplace_back(llvm::AttributeList::FunctionIndex, llvm::AttributeSet::get(ctx, fnAttrs));
        }
    
        llvm::AttributeList attrList = llvm::AttributeList::get(ctx, attrListPairs);
        function->setAttributes(attrList);

    }      

}
