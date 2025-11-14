/**
 * @file CollectionType.hpp
 * @brief Declares the CollectionType class, the abstract base for all collection and container types.
 * 
 * The CollectionType class extends the BaseType interface to represent container-like data structures 
 * such as arrays, lists, maps, and sets within the Lynx type system. It defines the common interface 
 * and semantics for both sequential (list-like) and associative (key-value) collections.
 * 
 * **Key Responsibilities:**
 * - Defines a uniform interface for collection manipulation (insertion, retrieval, iteration, clearing).
 * - Supports both sequential (index-based) and associative (key-based) collection models.
 * - Provides metadata about size, capacity, ordering, uniqueness, and homogeneity.
 * - Serves as the base class for specialized collection types such as ArrayType, MapType, and SetType.
 * 
 * **Design Notes:**
 * - Integrates with LLVM IR generation for runtime representation of collection structures.
 * - Relies on the AstContext for error reporting and type context resolution.
 * - Derived types must implement element/value management and cloning behavior.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_COLLECTION_TYPE_HPP
#define LYNX_COLLECTION_TYPE_HPP

#include <optional>
#include "BaseType.hpp"
#include <context/AstContext.hpp>

namespace LynxTypes {
    
    using namespace LynxContext;

    class CollectionType : public BaseType {

        protected:

            size_t numElements = 0;

            BaseType* elementType;

            BaseType* elementValue;            

        public:

            using ElementCallback = std::function<void(llvm::Value*)>;

            using KeyValueCallback = std::function<void(llvm::Value* key, llvm::Value* value)>;

        public:

            explicit CollectionType(AstContext* context) : BaseType(context) {}

            /**
             * @brief Returns false since collections are not built-in scalar types.
            */
            inline bool isCollectionType() const noexcept override { return true; }
        
            /**
             * @brief Returns true if this collection is associative (key-value based).
             */
            virtual inline bool isAssociative() const noexcept { return false; }

            /**
             * @brief Returns true if this collection is sequential (ordered list).
            */
            virtual inline bool isSequential() const noexcept { return false; }
                
            /**
             * @brief Returns true if the collection has a fixed size (e.g. fixed-length array).
            */
            virtual bool hasFixedSize() const noexcept { return false; }

            /**
             * @brief Returns the current number of elements in the collection.
             * @return Size as number of elements, or 0 if undefined.
            */
            virtual size_t getSize() const { return 0; } 

            inline void setSize(size_t eleSize) { numElements = eleSize; }

            /**
             * @brief Returns the capacity (max elements before resize needed) if applicable.
             * @return Optional size indicating capacity or empty if not applicable.
            */
            virtual std::optional<size_t> getCapacity() const { return {}; }

            /**
             * @brief Returns true if the collection can resize dynamically (e.g. vector).
            */
            virtual bool isResizable() const { return true; } 
        
            /**
             * @brief Returns a string describing the collection's shape or dimensions.
             * @return String representation of shape or empty if not applicable.
            */
            virtual std::string getShapeString() const { return ""; } 
        
            /**
             * @brief Returns true if all elements are of the same type.
            */
            virtual bool isHomogeneous() const { return true; } 

            /**
             * @brief Returns true if the collection preserves order of elements.
            */
            virtual bool isOrdered() const { return true; }  

            /**
             * @brief Returns true if the collection guarantees unique elements.
            */
            virtual bool isUnique() const { return false; }

            virtual void setElementType(BaseType* elementType) = 0;

            virtual const BaseType* getElementType() const = 0;

            virtual void setValueType(BaseType* elementValue) = 0;

            virtual const BaseType* getValueType() const = 0;

            virtual std::unique_ptr<BaseType> clone() const override = 0;

            /**
             * @brief Returns the element at the specified index (for sequential collections).
             * @param index LLVM value representing the index.
             * @return LLVM value of the element.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual llvm::Value* getElementAt(llvm::Value* index) {
                astContext->reportError(makeRuntimeError("getElementAt not supported for this collection type"));
                return nullptr;
            }

            /**
             * @brief Returns the value associated with a key (for associative collections).
             * @param key LLVM value representing the key.
             * @return LLVM value of the associated element.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual llvm::Value* getValueForKey(llvm::Value* key) {
                astContext->reportError(makeRuntimeError("getValueForKey not supported for this collection type"));
                return nullptr;
            }

            /**
             * @brief Inserts an element into the collection (for sequential collections).
             * @param element LLVM value of the element to insert.
             * @return LLVM value indicating success or result.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual llvm::Value* insertElement(llvm::Value* element) {
                astContext->reportError(makeRuntimeError("insertElement not supported for this collection type"));
                return nullptr;
            }

            /**
             * @brief Inserts a key-value pair into the collection (for associative collections).
             * @param key LLVM value of the key.
             * @param value LLVM value of the value.
             * @return LLVM value indicating success or result.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual llvm::Value* insertElement(llvm::Value* key, llvm::Value* value) {
                astContext->reportError(makeRuntimeError("insertElement not supported for this collection type"));
                return nullptr;
            }

            /**
             * @brief Clears all elements from the collection.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual void clear() {
                astContext->reportError(makeRuntimeError("clear not supported for this collection type"));
            }

            /**
             * @brief Iterates over all elements, invoking the provided callback (for sequential collections).
             * @param callback Function to be called for each element.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual void forEachElement(const ElementCallback& callback) {
                astContext->reportError(makeRuntimeError("forEachElement not supported for this collection type"));
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                auto o = dynamic_cast<const CollectionType*>(other);
                if (!o) return false;

                return false;
            }

            /**
             * @brief Iterates over all key-value pairs, invoking the provided callback (for associative collections).
             * @param callback Function to be called for each key-value pair.
             * @throws Runtime error if not supported by this collection type.
            */
            virtual void forEachKeyValue(const KeyValueCallback& callback) {
                astContext->reportError(makeRuntimeError("forEachKeyValue must be implemented by derived AssociativeType"));
            }  

    };
}

#endif


