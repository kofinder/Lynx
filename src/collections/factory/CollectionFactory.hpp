#ifndef LYNX_COLLECTION_FACTORY_HPP
#define LYNX_COLLECTION_FACTORY_HPP

#include <memory>
#include <stdexcept>

// Core interfaces
#include "interfaces/ICollection.hpp"
#include "interfaces/IMap.hpp"
#include "interfaces/IStack.hpp"
#include "interfaces/IQueue.hpp"

// Enum kind definitions
#include "kinds/CollectionKind.hpp"

// Concrete collection adapters
#include "adapter/ArrayAdapter.hpp"
#include "adapter/VectorAdapter.hpp"
#include "adapter/ListAdapter.hpp"
#include "adapter/SetAdapter.hpp"
#include "adapter/MapAdapter.hpp"
#include "adapter/DictAdapter.hpp"
#include "adapter/StackAdapter.hpp"
#include "adapter/QueueAdapter.hpp"
#include "adapter/TreeAdapter.hpp"
#include "adapter/GraphAdapter.hpp"

namespace LynxCollection {

    /**
     * @brief Factory for creating various collection types.
     *
     * Provides a unified interface to instantiate different data structure adapters
     * such as vector, array, set, tree, map, etc. These all conform to the ICollection or IMap family.
     */
    class CollectionFactory {
        public:
            CollectionFactory() = delete;

            /**
             * @brief Creates an ICollection<T> based on the CollectionKind.
             * 
             * @tparam T The data type stored in the collection.
             * @param kind The type of collection to create.
             * @return std::unique_ptr<ICollection<T>> A smart pointer to the created collection.
             */
            template <typename T>
            static std::unique_ptr<ICollection<T>> createCollection(CollectionKind kind) {
                switch (kind) {
                    case CollectionKind::ARRAY:
                        return std::make_unique<ArrayAdapter<T, 100>>();  // Fixed-size array
                    case CollectionKind::VECTOR:
                        return std::make_unique<VectorAdapter<T>>();
                    case CollectionKind::LIST:
                        return std::make_unique<ListAdapter<T>>();
                    case CollectionKind::SET:
                        return std::make_unique<SetAdapter<T>>();
                    case CollectionKind::TREE:
                        return std::make_unique<TreeAdapter<T>>();
                    case CollectionKind::GRAPH:
                        return std::make_unique<GraphAdapter<T>>();
                    default:
                        throw std::runtime_error("Unsupported collection kind for ICollection");
                }
            }

            /**
             * @brief Creates a key-value map (generic map interface).
             * 
             * @tparam K Key type.
             * @tparam V Value type.
             * @return std::unique_ptr<IMap<K, V>> A map supporting key-based access.
             */
            template <typename K, typename V>
            static std::unique_ptr<IMap<K, V>> createMap() {
                return std::make_unique<MapAdapter<K, V>>();
            }

            /**
             * @brief Creates a dictionary (map with string keys).
             *
             * @tparam V Value type.
             * @return std::unique_ptr<IMap<std::string, V>> Dictionary instance.
             */
            template <typename V>
            static std::unique_ptr<IMap<std::string, V>> createDict() {
                return std::make_unique<DictAdapter<std::string, V>>();
            }

            /**
             * @brief Creates a stack adapter.
             * 
             * @tparam T Element type.
             * @return std::unique_ptr<IStack<T>> Stack instance.
             */
            template <typename T>
            static std::unique_ptr<IStack<T>> createStack() {
                return std::make_unique<StackAdapter<T>>();
            }

            /**
             * @brief Creates a queue adapter.
             * 
             * @tparam T Element type.
             * @return std::unique_ptr<IQueue<T>> Queue instance.
             */
            template <typename T>
            static std::unique_ptr<IQueue<T>> createQueue() {
                return std::make_unique<QueueAdapter<T>>();
            }
    };

    } // namespace LynxCollection

#endif // LYNX_COLLECTION_FACTORY_HPP
