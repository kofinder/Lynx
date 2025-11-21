
file(GLOB_RECURSE BIZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE HANDLER_SOURCERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/handler/*.hpp"
)

add_library(LynxFunctionAttr STATIC ${BIZ_SOURCES} ${HANDLER_SOURCERS})

target_include_directories(LynxFunctionAttr PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/handler
    ${LIBRARY_INCLUDES}
)

target_link_libraries(LynxFunctionAttr PUBLIC LynxLogger)

message(STATUS "✅ LynxFunctionAttr module configured successfully! ")

