#pragma once

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
