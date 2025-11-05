set(LIB_LOGGER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/Logger.cpp
)

set(LIB_LOGGER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.hpp
)

set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(LynxLogger STATIC ${LIB_LOGGER_SOURCES} ${LIB_LOGGER_HEADERS})

target_include_directories(LynxLogger PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxLogger PUBLIC spdlog::spdlog cxxopts::cxxopts)
