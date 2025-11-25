function(get_lynx_version VERSION_FILE)
    file(READ "${VERSION_FILE}" lynx_ver)
    # message(STATUS "Reading version file content:")
    # message(STATUS "Contents of version file:\n${lynx_ver}")

    string(REGEX MATCH "#[ \t]*define[ \t]+LYNX_VERSION[ \t]+\"([0-9]+)\\.([0-9]+)\\.([0-9]+)([a-z0-9]*)\"" _ "${lynx_ver}")

    if(NOT DEFINED CMAKE_MATCH_1)
        message(FATAL_ERROR "Could not parse LYNX_VERSION from ${VERSION_FILE}")
    endif()

    set(LYNX_VERSION_MAJOR "${CMAKE_MATCH_1}" PARENT_SCOPE)
    set(LYNX_VERSION_MINOR "${CMAKE_MATCH_2}" PARENT_SCOPE)
    set(LYNX_VERSION_PATCH "${CMAKE_MATCH_3}" PARENT_SCOPE)
    set(LYNX_VERSION_SUFFIX "${CMAKE_MATCH_4}" PARENT_SCOPE)

    if("${LYNX_VERSION_SUFFIX}" MATCHES "dev")
        set(LYNX_DOC_REPO_TAG "main" PARENT_SCOPE)
    else()
        set(LYNX_DOC_REPO_TAG "v${LYNX_VERSION_MAJOR}.${LYNX_VERSION_MINOR}.${LYNX_VERSION_PATCH}${LYNX_VERSION_SUFFIX}" PARENT_SCOPE)
    endif()
endfunction()
