/**
 * @file TypeCaster.hpp
 * @brief Provides utility functions for safely casting and cloning `BaseType` objects.
 *
 * This header defines a set of templated functions to cast pointers or references
 * of `BaseType` to derived types. It also provides specialized overloads for
 * built-in, user-defined, sequential, associative, and wrapper types in the Lynx
 * type system. Additionally, it supports cloning a `BaseType` and casting the clone
 * to a specific derived type.
 *
 * These utilities facilitate type-safe operations and simplify type handling
 * across the compiler's semantic analysis, code generation, and type inference
 * components.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/


#ifndef LYNX_TYPE_CASTER_HPP
#define LYNX_TYPE_CASTER_HPP

#include <iostream>
#include <types/interfaces/BaseType.hpp>
#include <types/interfaces/CollectionType.hpp>

#include <types/associative/MapType.hpp>
#include <types/associative/DictionaryType.hpp>

#include <types/sequential/ArrayType.hpp>
#include <types/sequential/VectorType.hpp>
#include <types/sequential/ListType.hpp>
#include <types/sequential/QueueType.hpp>
#include <types/sequential/SetType.hpp>
#include <types/sequential/StackType.hpp>

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
#include <types/userdefined/MixinType.hpp>
#include <types/userdefined/InterfaceType.hpp>

#include <types/wrapper/ReferenceType.hpp>
#include <types/wrapper/PointerType.hpp>
#include <types/infrerence/AutoType.hpp>
#include <constants/MagicNumericConstants.hpp>

namespace LynxTypes::TypeCasting {

    using namespace LynxConstants;
    using namespace LynxContext;


    /**
     * @brief Casts a BaseType pointer to a derived type T.
     * @param base Pointer to base type.
     * @return Pointer to T if successful, nullptr otherwise.
     */
    template <typename T>
    static T* castType(BaseType* base) {
        return dynamic_cast<T*>(base);
    }

    /**
     * @brief Casts a BaseType reference to a derived type T pointer.
     * @param base Reference to base type.
     * @return Pointer to T if successful, nullptr otherwise.
     */
    template <typename T>
    static T* castType(BaseType& base) {
        return dynamic_cast<T*>(&base);
    }

    /**
     * @brief Casts a const BaseType reference to a const derived type pointer.
     * @param base Const reference to base type.
     * @return Pointer to const T if successful, nullptr otherwise.
     */
    template <typename T>
    static const T* castType(const BaseType& base) {
        return dynamic_cast<const T*>(&base);
    }

    /**
     * @brief Casts a const BaseType pointer to a const derived type pointer.
     * @param base Const pointer to base type.
     * @return Pointer to const T if successful, nullptr otherwise.
     */
    template <typename T>
    static const T* castType(const BaseType* base) {
        return dynamic_cast<const T*>(base);
    }

    /**
     * @brief Clones a BaseType pointer and casts it to a derived type T.
     * @param base Pointer to base type to clone.
     * @return std::unique_ptr<T> if clone and cast succeed, nullptr otherwise.
     */
    template <typename T, typename Base>
    static std::unique_ptr<T> cloneAs(const Base* base) {
        if (!base) return nullptr;
        auto cloned = base->clone();  // returns unique_ptr<BaseType>
        if (auto* casted = dynamic_cast<T*>(cloned.get())) {
            cloned.release();
            return std::unique_ptr<T>(casted);
        }
        return nullptr;
    }

    template <>
    inline const AutoType* castType<AutoType>(const BaseType* base) {
        if(!base) {
            std::cerr << "Base Type is null, doububle check please!";
            return nullptr;
        }
        return dynamic_cast<const AutoType*>(base);
    }

    template <>
    inline const ByteType* castType<ByteType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::BYTE) {
            return dynamic_cast<const ByteType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const ShortType* castType<ShortType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::SHORT) {
            return dynamic_cast<const ShortType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const IntegerType* castType<IntegerType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::INT) {
            return dynamic_cast<const IntegerType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const LongType* castType<LongType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::LONG) {
            return dynamic_cast<const LongType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const FloatType* castType<FloatType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::FLOAT) {
            return dynamic_cast<const FloatType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const DoubleType* castType<DoubleType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::DOUBLE) {
            return dynamic_cast<const DoubleType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const CharType* castType<CharType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::CHAR) {
            return dynamic_cast<const CharType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const BooleanType* castType<BooleanType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::BOOLEAN) {
            return dynamic_cast<const BooleanType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const StringType* castType<StringType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::STRING) {
            return dynamic_cast<const StringType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const VoidType* castType<VoidType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::VOID) {
            return dynamic_cast<const VoidType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const DateType* castType<DateType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::DATE) {
            return dynamic_cast<const DateType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const DateTimeType* castType<DateTimeType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::DATETIME) {
            return dynamic_cast<const DateTimeType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const FileType* castType<FileType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::FILE) {
            return dynamic_cast<const FileType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const EnumType* castType<EnumType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::ENUM) {
            return dynamic_cast<const EnumType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const ClassType* castType<ClassType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::CLAZZ) {
            return dynamic_cast<const ClassType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const InterfaceType* castType<InterfaceType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::INTERFACE) {
            return dynamic_cast<const InterfaceType*>(base);
        }
        return nullptr;
    }

    
    template <>
    inline const MixinType* castType<MixinType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::MIXIN) {
            return dynamic_cast<const MixinType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const FunctionType* castType<FunctionType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::FUNCTION) {
            return dynamic_cast<const FunctionType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const ReferenceType* castType<ReferenceType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::REFERENCE) {
            return dynamic_cast<const ReferenceType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const PointerType* castType<PointerType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::POINTER) {
            return dynamic_cast<const PointerType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const ArrayType* castType<ArrayType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::ARRAY) {
            return dynamic_cast<const ArrayType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const ListType* castType<ListType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::LIST) {
            return dynamic_cast<const ListType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const QueueType* castType<QueueType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::QUEUE) {
            return dynamic_cast<const QueueType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const SetType* castType<SetType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::SET) {
            return dynamic_cast<const SetType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const StackType* castType<StackType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::STACK) {
            return dynamic_cast<const StackType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const VectorType* castType<VectorType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::VECTOR) {
            return dynamic_cast<const VectorType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const MapType* castType<MapType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::VECTOR) {
            return dynamic_cast<const MapType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const DictionaryType* castType<DictionaryType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::VECTOR) {
            return dynamic_cast<const DictionaryType*>(base);
        }
        return nullptr;
    }

    template <>
    inline const CollectionType* castType<CollectionType>(const BaseType* base) {
        if (base && base->getTypeTag() == DataType::COLLECTION) {
            return dynamic_cast<const CollectionType*>(base);
        }
        return nullptr;
    }

} 

#endif

