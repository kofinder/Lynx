/**
 * @file CloneType.hpp
 * @brief Provides helper templates for cloning containers of `BaseType` pointers.
 *
 * The `Cloned` namespace contains utility templates for deep-copying or shallow-copying
 * collections of type pointers, typically used for type hierarchies in the Lynx compiler.
 *
 * **Key Responsibilities:**
 * - `cloneMapContainer`: Deep-clones a map container of pointers and inserts copies via a user-supplied function.
 * - `cloneVectorContainer`: Deep-clones a vector of pointers and inserts copies via a user-supplied function.
 * - `cloneVectorShallow`: Performs a shallow copy of a vector of pointers, maintaining the original pointer references.
 *
 * **Usage:**
 * - Supports cloning `std::unordered_map` or `std::vector` containers holding `BaseType` or derived type pointers.
 * - Ensures unique ownership of cloned objects using `std::unique_ptr`.
 *
 * **Notes:**
 * - Templates are generic and can be used with any container holding cloneable pointers.
 * - `AddFunc` must be a callable accepting either `(key, unique_ptr<ValueType>)` for maps or `(unique_ptr<ValueType>)` for vectors.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLONE_TYPE_HPP
#define LYNX_CLONE_TYPE_HPP

#include <memory>
#include <types/interfaces/BaseType.hpp>

namespace LynxTypes::Cloned {
   
    template <typename Container, typename AddFunc>
    void cloneMapContainer(const Container& src, AddFunc addFunc) {
        for (const auto& [name, ptr] : src) {
            if (ptr) {
                addFunc(name, std::unique_ptr<std::decay_t<decltype(*ptr)>>(ptr->clone().release()));
            }
        }
    }

    template <typename PtrContainer, typename AddFunc>
    void cloneVectorContainer(const PtrContainer& src, AddFunc addFunc) {
        for (const auto& ptr : src) {
            if (ptr) {
                addFunc(std::unique_ptr<std::decay_t<decltype(*ptr)>>(
                    static_cast<std::decay_t<decltype(*ptr)>*>(ptr->clone().release())
                ));
            }
        }
    }

    template <typename PtrContainer, typename AddFunc>
    void cloneVectorShallow(const PtrContainer& src, AddFunc addFunc) {
        for (const auto* ptr : src) {
            addFunc(ptr);
        }
    }
}

#endif 
