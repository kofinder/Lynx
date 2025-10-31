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


using namespace LynxConstants;
using namespace LynxContext;

namespace LynxTypes::TypeCasting {

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
    inline const AutoType* castType<AutoType>(const BaseType* type) {
        if(!type) {
            std::cerr << "Base Type is null, doububle check please!" << std::endl;
            return nullptr;
        }
        return static_cast<const AutoType*>(type);
    }

    template <>
    inline const ByteType* castType<ByteType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::BYTE) {
            return static_cast<const ByteType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const ShortType* castType<ShortType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::SHORT) {
            return static_cast<const ShortType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const IntegerType* castType<IntegerType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::INT) {
            return static_cast<const IntegerType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const LongType* castType<LongType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::LONG) {
            return static_cast<const LongType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const FloatType* castType<FloatType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::FLOAT) {
            return static_cast<const FloatType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const DoubleType* castType<DoubleType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::DOUBLE) {
            return static_cast<const DoubleType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const CharType* castType<CharType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::CHAR) {
            return static_cast<const CharType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const BooleanType* castType<BooleanType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::BOOLEAN) {
            return static_cast<const BooleanType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const StringType* castType<StringType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::STRING) {
            return static_cast<const StringType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const VoidType* castType<VoidType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::VOID) {
            return static_cast<const VoidType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const DateType* castType<DateType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::DATE) {
            return static_cast<const DateType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const DateTimeType* castType<DateTimeType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::DATETIME) {
            return static_cast<const DateTimeType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const FileType* castType<FileType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::FILE) {
            return static_cast<const FileType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const EnumType* castType<EnumType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::ENUM) {
            return static_cast<const EnumType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const ClassType* castType<ClassType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::CLAZZ) {
            return static_cast<const ClassType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const InterfaceType* castType<InterfaceType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::INTERFACE) {
            return static_cast<const InterfaceType*>(type);
        }
        return nullptr;
    }

    
    template <>
    inline const MixinType* castType<MixinType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::MIXIN) {
            return static_cast<const MixinType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const FunctionType* castType<FunctionType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::FUNCTION) {
            return static_cast<const FunctionType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const ReferenceType* castType<ReferenceType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::REFERENCE) {
            return static_cast<const ReferenceType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const PointerType* castType<PointerType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::POINTER) {
            return static_cast<const PointerType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const ArrayType* castType<ArrayType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::ARRAY) {
            return static_cast<const ArrayType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const ListType* castType<ListType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::LIST) {
            return static_cast<const ListType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const QueueType* castType<QueueType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::QUEUE) {
            return static_cast<const QueueType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const SetType* castType<SetType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::SET) {
            return static_cast<const SetType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const StackType* castType<StackType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::STACK) {
            return static_cast<const StackType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const VectorType* castType<VectorType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::VECTOR) {
            return static_cast<const VectorType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const MapType* castType<MapType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::VECTOR) {
            return static_cast<const MapType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const DictionaryType* castType<DictionaryType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::VECTOR) {
            return static_cast<const DictionaryType*>(type);
        }
        return nullptr;
    }

    template <>
    inline const CollectionType* castType<CollectionType>(const BaseType* type) {
        if (type && type->getTypeTag() == DataType::COLLECTION) {
            return static_cast<const CollectionType*>(type);
        }
        return nullptr;
    }

} 

#endif

