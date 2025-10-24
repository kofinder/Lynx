#include "gc_runtime.h"
#include <gc.h>
#include <cstdio>
#include <cstring>
#include <iostream>

extern "C" {

    static inline void register_finalizer(void* obj, void (*finalizer)(void*, void*), void* client_data) {
        if (!obj || !finalizer) return;
    
        GC_register_finalizer_no_order(
            obj,
            finalizer,
            client_data,
            NULL, // old finalizer
            NULL  // old client data
        );
    }
    
    void file_finalizer(void* obj, void* client_data) {
        if (!obj) return;
    
        typedef struct {
            char* path;
            FILE* handle;
        } File;
    
        File* file = (File*)obj;
    
        if (file->handle) {
            fclose(file->handle);
            file->handle = NULL;
        }
    
        if (file->path) {
            // GC-managed memory, no need to free explicitly
            file->path = NULL;
        }
    
        // Optional: debug message
        // printf("Finalized File object\n");
    }
    
}