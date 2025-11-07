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
#include "TypeMethodResolver.hpp"
#include "IntMethodResolver.hpp"
#include "DateTimeMethodResolver.hpp"
#include <constants/DataType.hpp>

#include "ByteMethodResolver.hpp"
#include "ShortMethodResolver.hpp"
#include "IntMethodResolver.hpp"
#include "LongMethodResolver.hpp"
#include "FloatMethodResolver.hpp"
#include "DoubleMethodResolver.hpp"
#include "CharacterMethodResolver.hpp"
#include "BoolMethodResolver.hpp"
#include "DateTimeMethodResolver.hpp"
#include "StringMethodResolver.hpp"
#include "AutoMethodResolver.hpp"


namespace LynxResolver {

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
                    case DataType::INT: return std::make_unique<IntMethodResolver>();
                    case DataType::LONG: return std::make_unique<LongMethodResolver>();
                    case DataType::FLOAT: return std::make_unique<FloatMethodResolver>();
                    case DataType::DOUBLE: return std::make_unique<DoubleMethodResolver>();
                    case DataType::CHAR: return std::make_unique<CharacterMethodResolver>();
                    case DataType::BOOLEAN: return std::make_unique<BoolMethodResolver>();
                    case DataType::STRING: return std::make_unique<StringMethodResolver>();
                    case DataType::DATETIME: return std::make_unique<DateTimeMethodResolver>();
                    case DataType::AUTO: return std::make_unique<AutoMethodResolver>();
                    default: return nullptr;
                }
            }
    };
        
}

#endif
