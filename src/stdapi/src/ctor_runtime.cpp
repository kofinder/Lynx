#include "ctor_runtime.h"
#include <gc.h>
#include <cstdio>
#include <cstring>
#include <iostream>

extern "C" {

    void int_ctor(int* ptr, int value) {
        if (ptr) {
            *ptr = value;
            std::cout << "[int_ctor] Initialized int at " << ptr << " with value " << value << "\n";
        }
    }
    
    void double_ctor(double* ptr, double value) {
        if (ptr) {
            *ptr = value;
            std::cout << "[double_ctor] Initialized double at " << ptr << " with value " << value << "\n";
        }
    }   
    
    void datetime_ctor(
        void* ptr,
        int year, int month, int day,
        int hour, int minute, int second,
        int ms, int tz
    ) {
        if (!ptr) {
            std::cerr << "[datetime_ctor] Error: null pointer provided\n";
            return;
        }

        datetime_t* dt = static_cast<datetime_t*>(ptr);
        dt->year   = year;
        dt->month  = month;
        dt->day    = day;
        dt->hour   = hour;
        dt->minute = minute;
        dt->second = second;
        dt->ms     = ms;
        dt->tz     = tz;

        std::cout << "[datetime_ctor] Initialized DateTime at " << dt
                  << " => " << year << "-" << month << "-" << day
                  << " " << hour << ":" << minute << ":" << second
                  << "." << ms << " tz=" << tz << "\n";
    }

    void file_ctor(void* ptr, const char* path) {
        if (!ptr || !path) {
            std::cerr << "[file_ctor] Error: null pointer provided\n";
            return;
        }
        
        file_t* file = reinterpret_cast<file_t*>(ptr);
        file->path = path;
        file->handle = fopen(path, "r+");
        if (file->handle) {
            std::cout << "[file_ctor] Opened file '" << path << "'\n";
            file->handle = fopen(path, "w+");
        } else {
            std::cerr << "[file_ctor] Failed to open file '" << path << "'\n";
        }    
    }
}