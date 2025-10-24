#ifndef LYNX_ASSOCIATIVE_TYPE_HPP
#define LYNX_ASSOCIATIVE_TYPE_HPP

#include "CollectionType.hpp"

namespace LynxTypes {

    /**
     * @class AssociativeType
     * @brief Abstract base class representing associative collections (key-value mappings).
     * 
     * Extends CollectionType to support key-value pairs, such as maps or dictionaries.
     * Defines interfaces for accessing keys and values, inserting elements, iteration, and clearing.
    */
    class AssociativeType : public CollectionType {

        public:
          
            /**
             * @brief Construct an AssociativeType with given key and value types.
             * @param context Shared AST context for error reporting and type info.
             * @param key Shared pointer to the key type.
             * @param value Shared pointer to the value type.
             */
            explicit AssociativeType(AstContext* context) : CollectionType(context) {}

            /**
             * @brief Returns true as this collection is associative by definition.
            */
            inline bool isAssociative() const override { return true; }

            inline bool supportsKeyLookup() const override { return true; }

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
            llvm::Value* createValue(LValueType value) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                auto o = dynamic_cast<const AssociativeType*>(other);
                if (!o) return false;
                switch (other->getTypeTag()) {
                    case DataType::MAP:    return true;
                    case DataType::DICT:   return true;
                    default: return false;
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
            llvm::Value* createValue(std::vector<llvm::Value*> values) const override {
                astContext->reportError(makeRuntimeError("createValue doesn't support this createValue signature."));
                return nullptr;
            }

            /**
             * @brief Retrieve the value associated with the given key.
             * @param key LLVM value representing the key to look up.
             * @return LLVM value of the associated value.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* getValueForKey(llvm::Value* key) override {
                astContext->reportError(makeRuntimeError("getValueForKey must be implemented by derived AssociativeType"));
                return nullptr;
            }

            /**
             * @brief Insert a key-value pair into the collection.
             * @param key LLVM value of the key.
             * @param value LLVM value of the value.
             * @return LLVM value indicating success or result.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* insertElement(llvm::Value* key, llvm::Value* value) override {
                astContext->reportError(makeRuntimeError("insertElement with key+value must be implemented by derived AssociativeType"));
                return nullptr;
            }

            /**
             * @brief Iterate over all key-value pairs in the collection.
             * @param callback Function to invoke for each key-value pair.
             * @throws Runtime error if not implemented in derived class.
            */
            virtual void forEachKeyValue(const KeyValueCallback& callback) override {
                astContext->reportError(makeRuntimeError("forEachKeyValue must be implemented by derived AssociativeType"));
            }

            /**
             * @brief Remove all key-value pairs from the collection.
             * @throws Runtime error if not implemented in derived class.
            */
            virtual void clear() override {
                astContext->reportError(makeRuntimeError("clear must be implemented by derived AssociativeType"));
            }

            ~AssociativeType() override = default;
    };
}

#endif
