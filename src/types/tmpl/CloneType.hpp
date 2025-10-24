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
