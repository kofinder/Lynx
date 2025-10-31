#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "file_runtime.h"


file_t* file_open(const char* path, const char* mode) {
    printf("Call file_open functions.....\n");
    fflush(stdout);  // <-- Ensure output appears immediately
    file_t* f = (file_t*)malloc(sizeof(file_t));
    if (!f) return NULL;
    f->handle = fopen(path, mode);
    if (!f->handle) {
        perror("fopen failed");  // Shows system error
        free(f);
        return NULL;
    }
    return f;
}

void* file_open_default(const char* path) {
    printf("Call file_open_default functions.....\n");
    fflush(stdout);
    return (void*)file_open(path, "w+");
}

void file_write(void* file, const char* content) {
    printf("Call file_write functions.....\n");
    fflush(stdout);
    file_t* f = (file_t*)file;
    if (f && f->handle) {
        fputs(content, f->handle);
        fflush(f->handle);
    }
}

int file_read(void* fileHandle, char* buffer, size_t size) {
    printf("Call file_read functions.....\n");
    fflush(stdout);

    if (!fileHandle || !buffer || size == 0) return -1;
    FILE* f = (FILE*)fileHandle;
    size_t bytesRead = fread(buffer, sizeof(char), size, f);
    return (int)bytesRead;
}


const char* file_read_all(void* file) {
    
    printf("Call file_read_all functions.....\n");
    fflush(stdout);

    file_t* f = (file_t*)file;
    if (!f || !f->handle) return NULL;

    fseek(f->handle, 0, SEEK_END);
    long size = ftell(f->handle);
    rewind(f->handle);

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) return NULL;

    fread(buffer, 1, size, f->handle);
    buffer[size] = '\0';
    return buffer;
}

void file_close(void* file) {
    printf("Call file_close functions.....\n");
    fflush(stdout);
    file_t* f = (file_t*)file;
    if (f && f->handle) {
        fclose(f->handle);
        f->handle = NULL;  // prevent double-close
    }
}


bool file_exists(const char* path) {
    FILE* f = fopen(path, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

bool file_delete(const char* path) {
    return remove(path) == 0;
}

int file_get_size(void* file) {
    file_t* f = (file_t*)file;
    if (!f || !f->handle) return -1;

    long current = ftell(f->handle);
    fseek(f->handle, 0, SEEK_END);
    long size = ftell(f->handle);
    fseek(f->handle, current, SEEK_SET);

    return (int)size;
}