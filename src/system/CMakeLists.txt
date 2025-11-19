file(GLOB_RECURSE SYSTEM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/commands/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/*.cpp"
)

file(GLOB_RECURSE SYSTEM_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/commands/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/modules/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tmpl/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.hpp"
)

add_library(LynxSystem STATIC ${SYSTEM_SOURCES} ${SYSTEM_HEADERS})

target_include_directories(LynxSystem PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${LIBRARY_INCLUDES}
)

target_link_libraries(LynxSystem PUBLIC
    LynxLogger
    LynxContext
    LynxConstants
    LynxAst
    LynxTypes
)

message(STATUS "✅ LynxSystem module configured ")


