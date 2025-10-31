#ifndef FILE_RUNTIME_H
#define FILE_RUNTIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include "runtime_types.h"

/**
 * @brief Opaque structure representing an open file.
 *
 * The full definition of this struct is hidden from the user.
 * Clients interact with this only through pointers (File* or void*),
 * providing encapsulation of the underlying file-handling logic.
 */


/**
 * @brief Opens a file in default mode ("w+").
 *
 * @param path The path to the file.
 * @return A void* pointer to the opaque File struct, or NULL on failure.
 *
 * This is a convenience wrapper for file_open with mode "w+".
 */
void* file_open_default(const char* path);

/**
 * @brief Writes a null-terminated string to the given file.
 *
 * @param file A void* pointer to the File.
 * @param content The null-terminated string to write.
 *
 * If the file or its internal handle is invalid, this function does nothing.
 */
void file_write(void* file, const char* content);


/**
 * @brief Reads up to `size` bytes from the file into a buffer.
 *
 * @param fileHandle The file handle returned by file_open_default.
 * @param buffer The buffer to store read bytes.
 * @param size The maximum number of bytes to read.
 * @return The number of bytes actually read, or a negative value on error.
 *
 * Note: The buffer must be allocated by the caller with at least `size` bytes.
 */
int file_read(void* fileHandle, char* buffer, size_t size);

/**
 * @brief Reads the entire content of the file.
 *
 * @param file A void* pointer to the File.
 * @return A newly allocated null-terminated string containing the file's contents,
 *         or NULL on failure.
 *
 * The caller is responsible for freeing the returned string.
 */
const char* file_read_all(void* file);

/**
 * @brief Closes the file and frees associated resources.
 *
 * @param file A void* pointer to the File.
 *
 * Safe to call with NULL; the function will do nothing in that case.
 * After calling, the file pointer becomes invalid and must not be reused.
 */
void file_close(void* file);

/**
 * @brief Checks if a file exists at the specified path.
 *
 * @param path The file path.
 * @return true if the file exists and is accessible, false otherwise.
 */
bool file_exists(const char* path);

/**
 * @brief Deletes a file at the specified path.
 *
 * @param path The file path.
 * @return true on success, false on failure.
 */
bool file_delete(const char* path);

/**
 * @brief Returns the size of the file in bytes.
 *
 * @param file A pointer to a File struct.
 * @return The file size in bytes, or -1 on failure.
 */
int file_get_size(void* file);

#ifdef __cplusplus
}
#endif

#endif // FILE_RUNTIME_H
