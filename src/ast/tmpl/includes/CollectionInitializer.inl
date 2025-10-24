#pragma once

#include <stdexcept>
#include <type_traits>
#include <sstream>
#include <ast/LiteralNode.hpp>
#include <ast/LiteralListNode.hpp>
#include <ast/LiteralMapNode.hpp>

namespace LynxAst::STL {

    template<typename T>
    T CollectionInitializer::extractScalarValue(const Node* node) {
        const auto* literalNode = dynamic_cast<const LiteralNode*>(node);
        if (!literalNode) {
            throw std::runtime_error("Expected a LiteralNode.");
        }

        const auto& val = literalNode->getLiteralValue();

        if (auto actual = std::get_if<T>(&val)) {
            return *actual;
        }

        std::ostringstream err;
        err << "Type mismatch: Cannot extract type " << typeid(T).name();
        throw std::runtime_error(err.str());
    }

    template<typename T, std::size_t N>
    std::array<T, N> CollectionInitializer::fromLiteralArray(const LiteralListNode& node) {
        std::array<T, N> result{};
        
        if (node.getValues()->size() != N) {
            throw std::runtime_error("Mismatched array size");
        }
    
        std::size_t i = 0;
        for (const auto& child : *node.getValues()) {
            result[i++] = extractScalarValue<T>(child.get());
        }
    
        return result;
    }

    // vector<T>
    template<typename T>
    std::vector<T> CollectionInitializer::fromLiteralVector(const LiteralListNode& node) {
        std::vector<T> result;
        for (const auto& child : *node.getValues()) {
            result.push_back(extractScalarValue<T>(child.get()));
        }
        return result;
    }

    // list<T>
    template<typename T>
    std::list<T> CollectionInitializer::fromLiteralListToList(const LiteralListNode& node) {
        std::list<T> result;
        for (const auto& child : *node.getValues()) {
            result.push_back(extractScalarValue<T>(child.get()));
        }
        return result;
    }

    // set<T>
    template<typename T>
    std::set<T> CollectionInitializer::fromLiteralListToSet(const LiteralListNode& node) {
        std::set<T> result;
        for (const auto& child : *node.getValues()) {
            result.insert(extractScalarValue<T>(child.get()));
        }
        return result;
    }

    // stack<T>
    template<typename T>
    std::stack<T> CollectionInitializer::fromLiteralListToStack(const LiteralListNode& node) {
        std::stack<T> result;
        for (auto it = node.getValues()->rbegin(); it != node.getValues()->rend(); ++it) {
            result.push(extractScalarValue<T>(it->get()));
        }
        return result;
    }

    // queue<T>
    template<typename T>
    std::queue<T> CollectionInitializer::fromLiteralListToQueue(const LiteralListNode& node) {
        std::queue<T> result;
        for (const auto& child : *node.getValues()) {
            result.push(extractScalarValue<T>(child.get()));
        }
        return result;
    }

    // map<K, V>
    template<typename K, typename V>
    std::map<K, V> CollectionInitializer::fromLiteralMap(const LiteralListNode& mapNode) {
        const auto* asMap = dynamic_cast<const LiteralMapNode*>(&mapNode);
        if (!asMap) {
            throw std::runtime_error("Expected LiteralMapNode for map conversion.");
        }

        std::map<K, V> result;

        for (const auto& pairPtr : *asMap->getPairs()) {
            const auto* pair = pairPtr.get();
            K key = extractScalarValue<K>(pair->getKey());
            V val = extractScalarValue<V>(pair->getValue());
            result[key] = val;
        }

        return result;
    }

}
