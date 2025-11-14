/**
 * @file TypeResolverFactory.hpp
 * @brief Factory for creating type-specific method resolvers in the Lynx language.
 *
 * The `TypeResolverFactory` class provides a static interface to create instances
 * of `TypeMethodResolver` for different `DataType` enumerations. This allows
 * centralized management of type-specific method resolution logic.
 *
 * Supported types include:
 * - BYTE, SHORT, INT, LONG, FLOAT, DOUBLE
 * - CHAR, BOOLEAN
 * - STRING, DATETIME
 * - AUTO
 *
 * Example usage:
 * @code
 * auto resolver = LynxResolver::TypeResolverFactory::forType(LynxConstants::DataType::INT);
 * if (resolver) {
 *     resolver->resolveMethod("toString");
 * }
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_TYPE_RESOLVER_FACTORY_HPP
#define LYNX_TYPE_RESOLVER_FACTORY_HPP

#include <memory>
#include <constants/DataType.hpp>
#include "TypeMethodResolver.hpp"
#include "methods/DateTimeMethodResolver.hpp"
#include "methods/ByteMethodResolver.hpp"
#include "methods/ShortMethodResolver.hpp"
#include "methods/IntMethodResolver.hpp"
#include "methods/LongMethodResolver.hpp"
#include "methods/FloatMethodResolver.hpp"
#include "methods/DoubleMethodResolver.hpp"
#include "methods/CharacterMethodResolver.hpp"
#include "methods/BoolMethodResolver.hpp"
#include "methods/DateTimeMethodResolver.hpp"
#include "methods/StringMethodResolver.hpp"
#include "methods/AutoMethodResolver.hpp"
#include "methods/EnumMethodResolver.hpp"
#include "methods/InterfaceMethodResolver.hpp"
#include "methods/ClassMethodResolver.hpp"
#include "methods/MixinMethodResolver.hpp"

namespace LynxTypes {

    using namespace LynxConstants;

    class TypeResolverFactory {

        public:
        
            /**
             * @brief Creates a method resolver for the specified type.
             *
             * @param type The enumerated DataType (e.g., INT, STRING, DATETIME).
             * @return A unique_ptr to the corresponding TypeMethodResolver, or nullptr if not supported.
            */
            static std::unique_ptr<TypeMethodResolver> forType(DataType type) {
                switch (type) {
                    case DataType::BYTE: return std::make_unique<ByteMethodResolver>();
                    case DataType::SHORT: return std::make_unique<ShortMethodResolver>();
                    case DataType::INT: return IntMethodResolver::create();
                    case DataType::LONG: return std::make_unique<LongMethodResolver>();
                    case DataType::FLOAT: return std::make_unique<FloatMethodResolver>();
                    case DataType::DOUBLE: return std::make_unique<DoubleMethodResolver>();
                    case DataType::CHAR: return std::make_unique<CharacterMethodResolver>();
                    case DataType::BOOLEAN: return std::make_unique<BoolMethodResolver>();
                    case DataType::STRING: return std::make_unique<StringMethodResolver>();
                    // case DataType::DATETIME: return std::make_unique<DateTimeMethodResolver>();
                    // case DataType::AUTO: return std::make_unique<AutoMethodResolver>();
                    // case DataType::ENUM: return std::make_unique<EnumMethodResolver>();
                    // case DataType::INTERFACE: return std::make_unique<InterfaceMethodResolver>();
                    // case DataType::CLAZZ: return std::make_unique<ClassMethodResolver>();
                    // case DataType::MIXIN: return std::make_unique<MixinMethodResolver>();
                    default: return nullptr;
                }
            }
    };
        
}

#endif
