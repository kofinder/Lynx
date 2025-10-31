
set(LIB_CONTEXT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/AstContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/VirtualTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/GlobalSymbolContext.cpp
)

set(LIB_CONTEXT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/AstContext.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VirtualTable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HeapAllocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GlobalSymbolContext.hpp
)

set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(LynxContext STATIC 
    ${LIB_CONTEXT_SOURCES} 
    ${LIB_CONTEXT_HEADERS}
)

target_include_directories(LynxContext PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxContext PRIVATE
    LynxAst
    LynxTypes
    LynxErrors
    LynxLogger 
    LynxConstants
)