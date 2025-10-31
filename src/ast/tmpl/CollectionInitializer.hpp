#ifndef LYNX_COLLECTION_INITIALIZER_HPP
#define LYNX_COLLECTION_INITIALIZER_HPP

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <list>
#include <string>
#include <ast/Node.hpp>

#include <ast/LiteralNode.hpp>
#include <ast/LiteralNode.hpp>

namespace LynxAst::STL {

    /**
     * @brief Utility class for converting literal AST nodes into concrete STL collections.
     */
    class CollectionInitializer {

        public:

            // array<T>
            template<typename T, std::size_t N>
            static std::array<T, N> fromLiteralArray(const LiteralListNode& node);

            // vector<T>
            template<typename T>
            static std::vector<T> fromLiteralVector(const LiteralListNode& node);

            // list<T>
            template<typename T>
            static std::list<T> fromLiteralListToList(const LiteralListNode& node);

            // set<T>
            template<typename T>
            static std::set<T> fromLiteralListToSet(const LiteralListNode& node);

            // stack<T>
            template<typename T>
            static std::stack<T> fromLiteralListToStack(const LiteralListNode& node);

            // queue<T>
            template<typename T>
            static std::queue<T> fromLiteralListToQueue(const LiteralListNode& node);

            // map<K, V>
            template<typename K, typename V>
            static std::map<K, V> fromLiteralMap(const LiteralListNode& mapNode);

        private:

            template<typename T>
            static T extractScalarValue(const Node* node);
    };

} // namespace LynxAst

#include "includes/CollectionInitializer.inl"

#endif // LYNX_COLLECTION_INITIALIZER_HPP
