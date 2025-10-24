#ifndef LYNX_SEQUENTIAL_TYPE_HPP
#define LYNX_SEQUENTIAL_TYPE_HPP

#include "CollectionType.hpp"

namespace LynxTypes {

    /**
     * @class SequentialType
     * @brief Abstract base class representing sequential collections (ordered lists or arrays).
     * 
     * Extends CollectionType to support collections where elements are accessed by numeric indices.
     * Defines interfaces for element access, iteration, and clearing the collection.
    */
    class SequentialType : public CollectionType {

        public:

            /**
             * @brief Construct a SequentialType with the given element type.
             * @param context Shared AST context for error reporting and type info.
             * @param type Shared pointer to the element type.
            */
            explicit SequentialType(AstContext* context) : CollectionType(context){};

            /**
             * @brief Returns true as this collection is sequential by definition.
            */
            inline bool isSequential() const override { return true; }

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
            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override {
                astContext->reportError(makeRuntimeError("createValue (K, V) doesn't support this createValue signature."));
                return nullptr;
            }

            bool canAccept(const BaseType* other) const override {
                if (equals(other)) return true;
                auto o = dynamic_cast<const SequentialType*>(other);
                if (!o) return false;
                switch (other->getTypeTag()) {
                    case DataType::ARRAY:    return true;
                    case DataType::LIST:   return true;
                    case DataType::SET:   return true;
                    case DataType::VECTOR:   return true;
                    case DataType::QUEUE:   return true;
                    case DataType::STACK:   return true;
                    default: return false;
                }

                return false;
            }

            /**
             * @brief Sets the value type for associative types.
             * 
             * Used to configure what type of values an associative container (e.g., map) stores.
             * 
             * @param elementValue Pointer to the type of values to store.
             * 
             * @note This method must be implemented by types derived from AssociativeType.
             */
            void setValueType(BaseType* elementValue) override {
                astContext->reportError(makeRuntimeError("setValueType must be implemented by derived AssociateType"));
            }

            /**
             * @brief Gets the value type for associative types.
             * 
             * Retrieves the type of values an associative container holds.
             * 
             * @return Pointer to the BaseType representing the value type.
             * 
             * @note This method must be implemented by types derived from AssociativeType.
             */
            const BaseType* getValueType() const override {
                astContext->reportError(makeRuntimeError("getValueType must be implemented by derived AssociateType"));
                return nullptr;
            }


            /**
             * @brief Retrieve the element at the specified index.
             * @param index LLVM value representing the index to access.
             * @return LLVM value of the element at the given index.
             * @throws Runtime error if not implemented in derived class.
            */
            llvm::Value* getElementAt(llvm::Value* index) override {
                astContext->reportError(makeRuntimeError("getElementAt must be implemented by derived SequentialType"));
                return nullptr;
            }

            /**
             * @brief Iterate over all elements in the collection.
             * @param callback Function to invoke for each element.
             * @throws Runtime error if not implemented in derived class.
            */
            void forEachElement(const ElementCallback& callback) override {
                astContext->reportError(makeRuntimeError("forEachElement must be implemented by derived SequentialType"));
            }

            /**
             * @brief Remove all elements from the collection.
             * @throws Runtime error if not implemented in derived class.
            */
            virtual void clear() override {
                astContext->reportError(makeRuntimeError("clear must be implemented by derived SequentialType"));
            }

            virtual std::unique_ptr<BaseType> clone() const override = 0;

            ~SequentialType() override = default;
    };
}

#endif
