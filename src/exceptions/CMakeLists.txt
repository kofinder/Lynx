set(LIB_EXCEPTIONS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/SemanticException.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/UnimplementedException.cpp
)

set(LIB_EXCEPTIONS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseException.hpp
)

set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(LynxExceptions STATIC ${LIB_EXCEPTIONS_SOURCES} ${LIB_EXCEPTIONS_HEADERS})

target_include_directories(LynxExceptions PUBLIC ${LIBRARY_INCLUDES})
