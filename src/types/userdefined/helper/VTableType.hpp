#ifndef LYNX_VTABLE_TYPE_HELPER_HPP
#define LYNX_VTABLE_TYPE_HELPER_HPP

#include <string>
#include <unordered_map>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>

namespace LynxTypes {

    struct VTableType {

        std::string vtableName;

        llvm::GlobalVariable* gvtable;

        llvm::StructType* structType; 
        
        std::unordered_map<std::string, unsigned> mapIdx;

    };

}

#endif