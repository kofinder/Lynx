#ifndef LYNX_CLONE_NODE_TEMPLATE_HPP
#define LYNX_CLONE_NODE_TEMPLATE_HPP

#include <iostream>
#include <string>
#include <optional>
#include <memory>
#include <vector>
#include "Node.hpp"
#include <llvm/IR/Function.h>

namespace LynxAst::Cloneable {

    /**
     * @brief Clone a unique_ptr to a vector of std::string.
     * 
     * Performs a deep copy of the vector<string> contents.
     * 
     * @param vec Constant reference to the unique_ptr holding the vector to clone.
     * @return std::unique_ptr<std::vector<std::string>> Cloned vector, or nullptr if input is nullptr.
    */
    inline std::unique_ptr<std::vector<std::string>> cloneStringVector(const std::unique_ptr<std::vector<std::string>>& vec) {
        if (!vec) return nullptr;
        return std::make_unique<std::vector<std::string>>(*vec);
    }


    /**
     * @brief Generic function template to clone a unique_ptr holding a node of type T.
     * 
     * This function performs a deep copy by invoking the `clone()` method of the
     * pointed-to node and casting the resulting pointer to the appropriate type.
     * 
     * @tparam T The concrete Node-derived type to clone.
     * @param node A constant reference to the unique_ptr holding the node to clone.
     * @return std::unique_ptr<T> A unique_ptr to the cloned node, or nullptr if input is nullptr.
    */
    template<typename T>
    std::unique_ptr<T> cloneNode(const std::unique_ptr<T>& node) {
        if (!node) return nullptr;
        return std::unique_ptr<T>(static_cast<T*>(node->clone().release()));
    }

    /**
     * @brief Specialization of cloneNode for the base Node type.
     * 
     * This specialization directly calls the clone method on the Node pointer
     * without any casting, returning the cloned unique_ptr<Node>.
     * 
     * @param node A constant reference to the unique_ptr<Node> to clone.
     * @return std::unique_ptr<Node> Cloned unique_ptr<Node>, or nullptr if input is nullptr.
    */
    template<>
    inline std::unique_ptr<Node> cloneNode<Node>(const std::unique_ptr<Node>& node) {
        if (!node) return nullptr;
        return node->clone();
    }
    

    /**
     * @brief Clone a unique_ptr to a vector of unique_ptr<Node>-derived objects.
     * 
     * Iterates through the vector, cloning each element individually using `cloneNode`.
     * Returns a unique_ptr to a new vector containing the cloned elements.
     * 
     * @tparam T The concrete Node-derived type contained in the vector.
     * @param vec Constant reference to the unique_ptr holding the vector to clone.
     * @return std::unique_ptr<std::vector<std::unique_ptr<T>>> Cloned vector of unique_ptrs, or nullptr if input is nullptr.
    */
    template<typename T>
    std::unique_ptr<std::vector<std::unique_ptr<T>>> cloneNodeVector(const std::unique_ptr<std::vector<std::unique_ptr<T>>>& vec) {
        if (!vec) return nullptr;
        auto clonedVec = std::make_unique<std::vector<std::unique_ptr<T>>>();
        for (const auto& elem : *vec) {
            clonedVec->push_back(cloneNode(elem));
        }
        return clonedVec;
    }

    /**
     * @brief Clone a vector (`vector<...>`) of unique_ptr<Node>-derived objects stored by value.
     *
     * Creates a new vector, clones each element using `cloneNode`, and returns
     * it by value. Move semantics ensure efficiency (RVO/move).
     *
     * @tparam T Concrete Node-derived type contained in the vector.
     * @param vec Constant reference to the vector to clone.
     * @return std::vector<std::unique_ptr<T>> New vector with deep-cloned elements.
    */
    template<typename T>
    std::vector<std::unique_ptr<T>> cloneNodeVector(const std::vector<std::unique_ptr<T>>& vec) {
        std::vector<std::unique_ptr<T>> clonedVec;
        clonedVec.reserve(vec.size());
        for (const auto& elem : vec) {
            clonedVec.push_back(cloneNode(elem));
        }
        return clonedVec;
    }


    /**
     * @brief Clone a std::optional holding a unique_ptr to a Node-derived object.
     * 
     * Returns std::nullopt if the optional is empty or contains a nullptr.
     * Otherwise, returns an optional containing the cloned unique_ptr.
     * 
     * @tparam T The concrete Node-derived type held by the unique_ptr.
     * @param opt Constant reference to the optional unique_ptr to clone.
     * @return std::optional<std::unique_ptr<T>> Optional holding the cloned unique_ptr or std::nullopt.
    */
    template<typename T>
    std::optional<std::unique_ptr<T>> cloneOptionalNode(const std::optional<std::unique_ptr<T>>& opt) {
        if (!opt.has_value() || !opt.value()) return std::nullopt;
        return cloneNode(opt.value());
    }

}

#endif
