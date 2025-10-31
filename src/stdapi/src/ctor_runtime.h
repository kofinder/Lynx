#ifndef CONSTRUCTOR_RUNTIME_H
#define CONSTRUCTOR_RUNTIME_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "runtime_types.h"

    void int_ctor(int* ptr, int value);

    void double_ctor(double* ptr, double value);

    /**
     * @brief Initializes a DateTime structure with provided values.
     * 
     * @param ptr Pointer to pre-allocated DateTime memory.
     * @param year Year (e.g., 2025)
     * @param month Month (1–12)
     * @param day Day (1–31)
     * @param hour Hour (0–23)
     * @param minute Minute (0–59)
     * @param second Second (0–59)
     * @param ms Milliseconds (0–999)
     * @param tz Timezone offset (e.g., 0 for UTC)
     */
    void datetime_ctor(
        void* ptr,
        int year, int month, int day,
        int hour, int minute, int second,
        int ms, int tz
    );

    /**
     * @brief Initializes a File structure with the given path.
     * 
     * @param ptr Pointer to pre-allocated File memory.
     * @param path Null-terminated string representing the file path.
    */
    void file_ctor(void* ptr, const char* path);

#ifdef __cplusplus
}
#endif

#endif // COLLECTION_RUNTIME_H
