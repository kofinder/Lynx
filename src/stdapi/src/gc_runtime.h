#ifndef LYNX_STDAPI_GC_RUNTIME_H
#define LYNX_STDAPI_GC_RUNTIME_H

#include <cstdint>


#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Allocate a GC-managed object (generic).
    */
    void* LYNX_GC_ALLOC_generic(uint64_t size) noexcept;

    /**
     * @brief Allocate a GC-managed object and record its type.
    */
    void* LYNX_GC_ALLOC_typed(uint64_t size, const char* typeName) noexcept;


#ifdef __cplusplus
}
#endif

#endif