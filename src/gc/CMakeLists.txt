find_library(GC_LIBRARY gc)
if(NOT GC_LIBRARY)
message(FATAL_ERROR "Boehm GC library not found. Please install libgc.")
endif()

# GC module sources
set(GC_SOURCES
    GCAllocator.cpp
    GCMutator.cpp
    GCCollector.cpp
    GCFinalizer.cpp
)

# GC module headers
set(GC_HEADERS
    GCGeneration.hpp
    ParallelGC.hp
    GCMutator.hpp
    GCAllocator.hpp
    GCCollector.hpp
    GCFinalizer.hpp
)

add_library(LynxGC STATIC ${GC_SOURCES} ${GC_HEADERS})

target_include_directories(LynxGC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LIBRARY_INCLUDES})

target_link_libraries(LynxGC PRIVATE ${GC_LIBRARY})

message(STATUS "GC module configured successfully")
