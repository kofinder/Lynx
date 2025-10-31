#ifndef LYNX_GRAPH_ADAPTER_HPP
#define LYNX_GRAPH_ADAPTER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T>
    class GraphAdapter : public ICollection<T> {
        // Adjacency list representation
        std::unordered_map<T, std::unordered_set<T>> adjacency;

    public:
        // ICollection<T> interface

        void add(const T& vertex) override {
            addVertex(vertex);
        }

        void remove(const T& vertex) override {
            removeVertex(vertex);
        }

        bool contains(const T& vertex) const override {
            return hasVertex(vertex);
        }

        std::size_t size() const override {
            return adjacency.size();
        }

        void clear() override {
            adjacency.clear();
        }

        // Graph-specific methods

        void addVertex(const T& vertex) {
            adjacency.try_emplace(vertex);
        }

        void addEdge(const T& from, const T& to) {
            addVertex(from);
            addVertex(to);
            adjacency[from].insert(to);
            // For undirected graph, also:
            // adjacency[to].insert(from);
        }

        bool hasVertex(const T& vertex) const {
            return adjacency.find(vertex) != adjacency.end();
        }

        bool hasEdge(const T& from, const T& to) const {
            auto it = adjacency.find(from);
            if (it == adjacency.end()) return false;
            return it->second.find(to) != it->second.end();
        }

        void removeVertex(const T& vertex) {
            adjacency.erase(vertex);
            for (auto& [_, neighbors] : adjacency) {
                neighbors.erase(vertex);
            }
        }

        void removeEdge(const T& from, const T& to) {
            auto it = adjacency.find(from);
            if (it != adjacency.end()) {
                it->second.erase(to);
            }
            // For undirected graph, also:
            // adjacency[to].erase(from);
        }

        // Optional: get neighbors of a vertex
        std::vector<T> neighbors(const T& vertex) const {
            std::vector<T> result;
            auto it = adjacency.find(vertex);
            if (it != adjacency.end()) {
                result.insert(result.end(), it->second.begin(), it->second.end());
            }
            return result;
        }
    };

}

#endif // LYNX_GRAPH_ADAPTER_HPP
