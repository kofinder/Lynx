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

using namespace LynxConstants;

namespace LynxResolver {

    /**
     * @class TypeResolverFactory
     * @brief Factory for creating the correct TypeMethodResolver instance based on DataType.
     *
     * This class provides a single static method `forType()` that acts as a dispatcher for type-based
     * method resolution. It returns a unique pointer to a subclass of TypeMethodResolver.
     */

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
