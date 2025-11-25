configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_BINARY_DIR}/configured_files/include/config.hpp"
    @ONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.rc.in"
    "${CMAKE_BINARY_DIR}/configured_files/config.rc"
    @ONLY
)

include_directories("${CMAKE_BINARY_DIR}/configured_files/include")
