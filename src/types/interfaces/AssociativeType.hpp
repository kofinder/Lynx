/**
 * @file AssociativeType.hpp
 * @brief Declares the AssociativeType class, the abstract base for all key-value collection types.
 *
 * The AssociativeType class extends CollectionType to represent associative data structures such as
 * maps, dictionaries, and hash tables within the Lynx type system. It defines the common interface
 * for containers that store and access elements via keys rather than sequential indices.
 *
 * **Key Responsibilities:**
 * - Defines the base interface for associative container types (Map, Dict, etc.).
 * - Provides virtual methods for inserting, retrieving, and iterating over key-value pairs.
 * - Integrates with LLVM to support runtime code generation for key-based lookups and mutations.
 * - Supports compatibility checks with other associative container types.
 *
 * **Intended Derivatives:**
 * - MapType
 * - DictType
 * - HashTableType
 *
 * **Design Notes:**
 * - This class complements SequentialType, forming the foundation of Lynx's collection hierarchy.
 * - Derived types must implement `getValueForKey()`, `insertElement(key, value)`, and `forEachKeyValue()`.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_ASSOCIATIVE_TYPE_HPP
#define LYNX_ASSOCIATIVE_TYPE_HPP

#include "CollectionType.hpp"

namespace LynxTypes {

    class AssociativeType : public CollectionType {

        public:
          
            /**
             * @brief Construct an AssociativeType with given key and value types.
             * @param context Shared AST context for error reporting and type info.
             * @param key Shared pointer to the key type.
             * @param value Shared pointer to the value type.
             */
            explicit AssociativeType(AstContext* context) : CollectionType(context) {}
            
            // Rule of five: allow default destructor, delete others
            ~AssociativeType() override = default;
            AssociativeType(const AssociativeType&) = delete;
            AssociativeType& operator=(const AssociativeType&) = delete;
            AssociativeType(AssociativeType&&) = delete;
            AssociativeType& operator=(AssociativeType&&) = delete;

            /**
             * @brief Returns true as this collection is associative by definition.
            */
            bool isAssociative() const noexcept override { return true; }

            /**
             * @brief Returns true as this collection is sequential by definition.
            */
            bool supportsKeyLookup() const noexcept override { return true; }

            /**
             * @brief Creates an LLVM value from a generic LValueType variant.
             * 
             * This overload is expected to handle primitive or literal values (e.g., int, bool).
             * 
             * @param value The value to convert to an LLVM representation.
             * @return LLVM value corresponding to the provided input.
             * 
             * @note This default implementation throws a runtime error and must be overridden by the appropriate type.
            */
            llvm::Value* createValue(LValueType /*unused*/) const override { return nullptr; }

            /**
             * @brief Checks whether this collection type can accept another type.
             * @param other Pointer to another BaseType.
             * @return True if the other type is compatible with this collection.
            */
            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                const auto* obj = dynamic_cast<const AssociativeType*>(other);
                if (!obj) return false;
                switch (other->getTypeTag()) {
                    case DataType::MAP:
                    case DataType::DICT:   
                        return true;
                    default: 
                        return false;
                }

                return false;
            }

            /**
             * @brief Creates an LLVM value from a list of LLVM values.
             * 
             * This overload is intended for structured or composite value construction (e.g., tuples, structs).
             * 
             * @param values A vector of LLVM values to be used in the construction.
             * @return LLVM value representing the constructed value.
             * 
             * @note This default implementation throws a runtime error, and should be overridden by supported types.
            */
            llvm::Value* createValue(std::vector<llvm::Value*> /*unused*/) const override { return nullptr; }

            /**
             * @brief Retrieve the value associated with the given key.
             * @param key LLVM value representing the key to look up.
             * @return LLVM value of the associated value.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* getValueForKey(llvm::Value* /*unused*/) override { return nullptr; }

            /**
             * @brief Insert a key-value pair into the collection.
             * @param key LLVM value of the key.
             * @param value LLVM value of the value.
             * @return LLVM value indicating success or result.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* insertElement(llvm::Value* /*unused*/, llvm::Value* /*unused*/) override { return nullptr; }

            /**
             * @brief Iterate over all key-value pairs in the collection.
             * @param callback Function to invoke for each key-value pair.
             * @throws Runtime error if not implemented in derived class.
            */
            void forEachKeyValue(const KeyValueCallback& /*unused*/) override {}
    };
}

#endif
