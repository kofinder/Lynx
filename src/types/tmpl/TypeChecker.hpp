/**
 * @file TypeChecker.hpp
 * @brief Provides LLVM IR type-checking utilities for Lynx types.
 *
 * This header defines templated functions to verify if an LLVM `Type*` corresponds
 * to a specific Lynx type, including built-in, user-defined, sequential, associative,
 * and wrapper types. These utilities assist in semantic analysis, code generation,
 * and type inference by enabling safe runtime type inspection.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_CHECKER_HPP
#define LYNX_TYPE_CHECKER_HPP

#include <iostream>

#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>


#include <types/sequential/ArrayType.hpp>
#include <types/sequential/VectorType.hpp>
#include <types/sequential/ListType.hpp>
#include <types/sequential/QueueType.hpp>
#include <types/sequential/SetType.hpp>
#include <types/sequential/StackType.hpp>

#include <types/associative/MapType.hpp>
#include <types/associative/DictionaryType.hpp>

#include <types/builtins/ByteType.hpp>
#include <types/builtins/ShortType.hpp>
#include <types/builtins/IntegerType.hpp>
#include <types/builtins/LongType.hpp>
#include <types/builtins/FloatType.hpp>
#include <types/builtins/DoubleType.hpp>
#include <types/builtins/CharType.hpp>
#include <types/builtins/BooleanType.hpp>
#include <types/builtins/StringType.hpp>
#include <types/builtins/VoidType.hpp>

#include <types/userdefined/EnumType.hpp>
#include <types/userdefined/DateType.hpp>
#include <types/userdefined/DateTimeType.hpp>
#include <types/userdefined/FileType.hpp>
#include <types/userdefined/FunctionType.hpp>
#include <types/userdefined/ClassType.hpp>
#include <types/wrapper/ReferenceType.hpp>
#include <types/wrapper/PointerType.hpp>
#include <types/infrerence/AutoType.hpp>

namespace LynxTypes::TypeChecker {

    template <typename T>
    static bool is(llvm::Type* type) {
        return llvm::isa<T>(type);
    }

    template <>
    inline bool is<ByteType>(llvm::Type* type) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            if (structType->getNumElements() == 1) {
                if (structType->getElementType(0)->isIntegerTy(8)) {
                    if (structType->getName() == MetadataTypeConstants::structureByteType) {
                        return true;
                    }
                }
            }
        }
        return false;    
    }

    template <>
    inline bool is<ShortType>(llvm::Type* type) {
        return type && type->isIntegerTy(16);
    }

    template <>
    inline bool is<IntegerType>(llvm::Type* type) {
        return type && type->isIntegerTy(32);
    }

    template <>
    inline bool is<LongType>(llvm::Type* type) {
        return type && type->isIntegerTy(64);
    }

    template <>
    inline bool is<FloatType>(llvm::Type* type) {
        return type && type->isFloatTy();
    }

    template <>
    inline bool is<DoubleType>(llvm::Type* type) {
        return type && type->isDoubleTy();
    }

    template <>
    inline bool is<CharType>(llvm::Type* type) {
        // if (auto* pointerType = llvm::dyn_cast<llvm::PointerType>(type)) {
        //     if (auto* structType = llvm::dyn_cast<llvm::StructType>(pointerType->getElementType())) {
        //         return structType->getName() == MetadataTypeConstants::structureCharType;
        //     }    
        // }
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            if (structType->getNumElements() == 1) {
                if (structType->getElementType(0)->isIntegerTy(8)) {
                    return structType->getName() == MetadataTypeConstants::structureCharType;
                }
            }
        }
        return false;
    }

    template <>
    inline bool is<BooleanType>(llvm::Type* type) {
        return type && type->isIntegerTy(1);
    }
    
    template <>
    inline bool is<StringType>(llvm::Type* type) {
        if (!type) return false;

        // if (type->isPointerTy()) {
        //     auto* elementType = type->getPointerElementType();
        //     return elementType && elementType->isIntegerTy(8);
        // }
    
        if (type->isArrayTy()) {
            return type->getArrayElementType()->isIntegerTy(8);
        }
    
        return false;    
    }

    template <>
    inline bool is<VoidType>(llvm::Type* type) {
        return type && type->isVoidTy();
    }

    template <>
    inline bool is<DateType>(llvm::Type* type) {

        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            return structType->getName() == MetadataTypeConstants::dateType;
        }

        return false;
    }

    template <>
    inline bool is<DateTimeType>(llvm::Type* type) {
        // if(auto pointerType = llvm::dyn_cast<llvm::PointerType>(type)) {
        //     if (auto* structType = llvm::dyn_cast<llvm::StructType>(pointerType->getPointerElementType())) {
        //         return structType->getName() == MetadataTypeConstants::structureDateTimeType;
        //     }    
        // }

        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            return structType->getName() == MetadataTypeConstants::dateTimeType;
        }

        return false;
    }

    template <>
    inline bool is<FileType>(llvm::Type* type) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            return structType->getName() == MetadataTypeConstants::fileType;
        }
        return false;
    }

    template <>
    inline bool is<EnumType>(llvm::Type* type) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {

            std::cout << "struct type ===========>:" << structType->getName().str() << std::endl;

            if (!structType || !structType->hasName()) return false;

            std::string name = structType->getName().str();
        
            constexpr std::string_view prefix = "enum.";
            if (name.compare(0, prefix.size(), prefix) != 0) return false;
                
            unsigned numElements = structType->getNumElements();
            if (numElements != 2) {
                std::cout << "[is<EnumType>] Unexpected number of elements: " << numElements << "\n";
                return false;
            }
            
            llvm::Type* first = structType->getElementType(0);
            llvm::Type* second = structType->getElementType(1);
        
            if (!first->isIntegerTy(64)) return false;

            if (auto* secondStruct = llvm::dyn_cast<llvm::StructType>(second)) {
                if (secondStruct->getNumElements() != 3) return false;
            
                auto* intType = secondStruct->getElementType(0);
                auto* charType = secondStruct->getElementType(1);
                auto* stringType = secondStruct->getElementType(2);
            
                // With opaque pointers, you cannot check the pointee type.
                bool validSecondStruct =
                    intType->isPointerTy() &&  // opaque pointer
                    charType->isIntegerTy(8) &&
                    stringType->isIntegerTy(64);
            
                return validSecondStruct;
            }
            
            return false;
        }

        return false;
    }

    template <>
    inline bool is<ClassType>(llvm::Type* type) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(type)) {
            if (structType->getName() == MetadataTypeConstants::classType) {
                return true;
            }
        }
        return false;
    }

    template <>
    inline bool is<FunctionType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<ReferenceType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<PointerType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<ArrayType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<ListType>(llvm::Type* type) {
       return false;
    }

    template <>
    inline bool is<QueueType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<SetType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<StackType>(llvm::Type* type) {
       return false;
    }

    template <>
    inline bool is<VectorType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<MapType>(llvm::Type* type) {
        return false;
    }

    template <>
    inline bool is<DictionaryType>(llvm::Type* type) {
        return false;
    }
}

#endif 