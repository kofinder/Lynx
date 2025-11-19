

set(LIB_MODULE_LINKER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Linker.hpp)

set(LIB_MODULE_LINKER_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/impl/Linker.cpp)

add_library(LynxLinker STATIC ${LIB_MODULE_LINKER_SOURCES} ${LIB_MODULE_LINKER_HEADER})

target_link_libraries(LynxLinker PRIVATE LynxLogger)

target_include_directories(LynxLinker PUBLIC ${LIBRARY_INCLUDES})



