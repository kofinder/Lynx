
set(LIB_MODULE_RESOLVER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/AutoMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ByteMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ShortMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/IntMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/LongMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/FloatMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/DoubleMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/BoolMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/StringMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/CharacterMethodResolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/DateTimeMethodResolver.cpp
)

set(LIB_MODULE_RESOLVER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeVisitor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TypeResolverFactory.hpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/AutoMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ByteMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShortMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IntMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LongMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FloatMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DoubleMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BoolMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StringMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CharacterMethodResolver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DateTimeMethodResolver.hpp
)

add_library(LynxResolver STATIC ${LIB_MODULE_RESOLVER_SOURCES} ${LIB_MODULE_RESOLVER_HEADERS})

target_include_directories(LynxResolver PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxResolver PUBLIC 
    spdlog::spdlog
    cxxopts::cxxopts 
    LynxLogger 
    LynxExceptions 
    LynxTypes 
    LynxConstants 
    LynxContext
    LynxAst
)
