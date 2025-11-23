/**
 * @file SequentialType.hpp
 * @brief Declares the SequentialType class, the abstract base for all ordered collection types.
 * 
 * The SequentialType class extends CollectionType to represent ordered collections such as arrays, 
 * lists, vectors, queues, and stacks within the Lynx type system. It provides the foundational 
 * interface and behavior common to all sequential containers, supporting element access, iteration, 
 * and structural operations.
 * 
 * **Key Responsibilities:**
 * - Defines the interface for sequential data structures (index-based access, iteration, clearing).
 * - Establishes type compatibility rules for related sequential collection types.
 * - Provides default error handling for unsupported operations, ensuring derived classes override them properly.
 * - Integrates with LLVM IR generation through the AstContext for semantic validation and code generation.
 * 
 * **Typical Derived Types:**
 * - ArrayType, VectorType, ListType, QueueType, StackType
 * 
 * **Design Notes:**
 * - All sequential containers are ordered and homogeneous by design.
 * - Derived classes must implement core operations such as element retrieval, insertion, and cloning.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SEQUENTIAL_TYPE_HPP
#define LYNX_SEQUENTIAL_TYPE_HPP

#include "CollectionType.hpp"

namespace LynxTypes {

    class SequentialType : public CollectionType {

        public:

            /**
             * @brief Construct a SequentialType with the given element type.
             * @param context Shared AST context for error reporting and type info.
             * @param type Shared pointer to the element type.
            */
            explicit SequentialType(AstContext* context) : CollectionType(context){};
            
            // Rule of five: allow default destructor, delete others
            ~SequentialType() override = default;
            SequentialType(const SequentialType&) = delete;
            SequentialType& operator=(const SequentialType&) = delete;
            SequentialType(SequentialType&&) = delete;
            SequentialType& operator=(SequentialType&&) = delete;

            /**
             * @brief Returns true as this collection is sequential by definition.
            */
            bool isSequential() const noexcept override { return true; }

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
             * @brief Creates an LLVM value from a list of key-value pairs.
             * 
             * Typically used by associative container types like maps or dictionaries.
             * 
             * @param pairs A vector of pairs representing keys and their corresponding values.
             * @return LLVM value representing the constructed associative type.
             * 
             * @note This default implementation throws a runtime error and must be overridden by types supporting key-value construction.
             */
            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> /*unused*/) const override { return nullptr; }

            /**
             * @brief Determines if this sequential type can accept another type.
             *
             * Checks for type equality first. If not equal, attempts a dynamic cast
             * to a SequentialType and then checks if the type tag corresponds to a
             * supported sequential collection (array, list, set, vector, queue, stack).
             *
             * @param other Pointer to another BaseType to check compatibility with.
             * @return True if the type can be accepted, false otherwise.
            */
            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                const auto* obj = dynamic_cast<const SequentialType*>(other);
                if (!obj) return false;
                switch (other->getTypeTag()) {
                    case DataType::ARRAY:
                    case DataType::LIST:
                    case DataType::SET:
                    case DataType::VECTOR:
                    case DataType::QUEUE:
                    case DataType::STACK: 
                        return true;
                    default: 
                        return false;
                }

                return false;
            }
            
            /**
             * @brief Retrieve the element at the specified index.
             * @param index LLVM value representing the index to access.
             * @return LLVM value of the element at the given index.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* getElementAt(llvm::Value* /*unused*/) override { return nullptr; }

            /**
             * @brief Iterate over all elements in the collection.
             * @param callback Function to invoke for each element.
             * @throws Runtime error if not implemented in derived class.
            */
            void forEachElement(const ElementCallback& /*unused*/) override { }
    };
}

#endif
