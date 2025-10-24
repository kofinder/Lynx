set(LIB_MANGLING_CONSTANT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/constants/ItaniumManglerPrefix.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constants/ItaniumTypeEncoding.hpp
)

set(LIB_MANGLING_UTILS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/TypeEncoderUtils.hpp
)

set(LIB_MANGLING_KIND_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/kinds/ItaniumMangler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kinds/MicrosoftMangler.hpp
)

set(LIB_MANGLING_HEADER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/IMangleStrategy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ManglerFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ManglingEngine.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ManglingContext.hpp
)

add_library(LynxMangler STATIC
    ${LIB_MANGLING_CONSTANT_SOURCES}
    ${LIB_MANGLING_UTILS_SOURCES}
    ${LIB_MANGLING_KIND_SOURCES}
    ${LIB_MANGLING_HEADER_SOURCES}
)

target_include_directories(LynxMangler PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxMangler PRIVATE LynxLogger LynxConstants LynxTypes LynxAst)