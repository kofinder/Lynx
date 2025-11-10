#include "gc_runtime.h"
#include <cstdio>
#include <cstdint>
#include <cstdbool>
#include <cstring>
#include <limits>
#include <cstdlib>

char IO_GET_CHAR_VALIDATED(const char* allowedChars) {
    std::printf("Enter a char value: ");

    char c;
    bool valid = false;
    while (!valid) {
        std::scanf(" %c", &c);
        for (size_t i = 0; allowedChars[i] != '\0'; ++i) {
            if (c == allowedChars[i]) {
                valid = true;
                break;
            }
        }
        if (!valid) {
            std::fprintf(stderr, "Invalid character! Allowed: %s\n", allowedChars);
            std::printf("Enter a char value: ");
        }
    }
    return c;
}

