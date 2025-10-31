#ifndef LYNX_TREE_ADAPTER_HPP
#define LYNX_TREE_ADAPTER_HPP

#include <memory>
#include <queue>
#include <vector>
#include <interfaces/ICollection.hpp>

namespace LynxCollection {

    template <typename T>
    class TreeAdapter : public ICollection<T> {
    private:
        struct Node {
            T value;
            std::unique_ptr<Node> left;
            std::unique_ptr<Node> right;

            Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
        };

        std::unique_ptr<Node> root;
        std::size_t nodeCount = 0;

    public:
        TreeAdapter() : root(nullptr), nodeCount(0) {}

        void add(const T& value) override {
            root = insert(std::move(root), value);
        }

        void remove(const T& value) override {
            bool erased = false;
            root = remove(std::move(root), value, erased);
            if (erased) --nodeCount;
        }

        bool contains(const T& value) const override {
            return contains(root.get(), value);
        }

        std::size_t size() const override {
            return nodeCount;
        }

        void clear() override {
            root.reset();
            nodeCount = 0;
        }

        // Extra utility
        std::vector<T> traverseLevelOrder() const {
            std::vector<T> result;
            if (!root) return result;
            std::queue<const Node*> q;
            q.push(root.get());

            while (!q.empty()) {
                const Node* node = q.front();
                q.pop();
                result.push_back(node->value);
                if (node->left) q.push(node->left.get());
                if (node->right) q.push(node->right.get());
            }

            return result;
        }

    private:
        std::unique_ptr<Node> insert(std::unique_ptr<Node> node, const T& value) {
            if (!node) {
                ++nodeCount;
                return std::make_unique<Node>(value);
            }

            if (value < node->value)
                node->left = insert(std::move(node->left), value);
            else if (value > node->value)
                node->right = insert(std::move(node->right), value);
            // Ignore duplicates

            return node;
        }

        std::unique_ptr<Node> remove(std::unique_ptr<Node> node, const T& value, bool& erased) {
            if (!node) return nullptr;

            if (value < node->value) {
                node->left = remove(std::move(node->left), value, erased);
            } else if (value > node->value) {
                node->right = remove(std::move(node->right), value, erased);
            } else {
                erased = true;
                if (!node->left) return std::move(node->right);
                if (!node->right) return std::move(node->left);

                Node* minLargerNode = findMin(node->right.get());
                node->value = minLargerNode->value;
                node->right = remove(std::move(node->right), node->value, erased);
            }

            return node;
        }

        Node* findMin(Node* node) {
            while (node && node->left) {
                node = node->left.get();
            }
            return node;
        }

        bool contains(const Node* node, const T& value) const {
            if (!node) return false;
            if (value == node->value) return true;
            if (value < node->value) return contains(node->left.get(), value);
            return contains(node->right.get(), value);
        }
    };

} // namespace LynxCollection

#endif // LYNX_TREE_ADAPTER_HPP
