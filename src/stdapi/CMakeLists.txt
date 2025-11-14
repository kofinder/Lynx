# ------------------------------------------
# Find required packages
# ------------------------------------------
find_package(Threads REQUIRED)

# Try to find Boehm GC (shared or PIC static)
find_library(GC_LIBRARY gc)
if(NOT GC_LIBRARY)
    message(FATAL_ERROR "Boehm GC library not found. Please install libgc (with -fPIC if static).")
endif()

message(STATUS "Boehm GC Library: ${GC_LIBRARY}")

# ------------------------------------------
# Source files
# ------------------------------------------
set(SRC_FILES
    src/runtime_types.h
    src/ctor_runtime.h
    src/ctor_runtime.cpp
    src/file_runtime.h
    src/file_runtime.c
    # src/collection_runtime.h
    # src/collection_runtime.cpp

    src/io_runtime.h
    src/io_runtime.cpp

    src/gc_runtime.h
    src/gc_runtime.cpp
)

# ------------------------------------------
# Build shared library
# ------------------------------------------
add_library(LynxBridge SHARED ${SRC_FILES})

# Include directories
target_include_directories(LynxBridge PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(LynxBridge PUBLIC gc pthread LynxCore)


# # Optional: set position independent code (ensures PIC for shared objects)
set_target_properties(LynxBridge PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ------------------------------------------
# Message
# ------------------------------------------
message(STATUS "LynxRuntime module configured successfully")
