function(target_set_warnings TARGET ENABLE ENABLE_AS_ERRORS)
    if(NOT ENABLE)
        message(STATUS "⚠️ Warnings disabled for target: ${TARGET}")
        return()
    endif()

    set(MSVC_WARNINGS
        /W4
        /permissive-)

    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Wpedantic)

    set(GCC_WARNINGS ${CLANG_WARNINGS})

    if(ENABLE_AS_ERRORS)
        list(APPEND MSVC_WARNINGS /WX)
        list(APPEND CLANG_WARNINGS -Werror)
        list(APPEND GCC_WARNINGS -Werror)
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(WARNINGS ${MSVC_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(WARNINGS ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(WARNINGS ${GCC_WARNINGS})
    else()
        message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
        set(WARNINGS "")
    endif()

    if(WARNINGS)
        target_compile_options(${TARGET} PRIVATE ${WARNINGS})
        message(STATUS "✅ Warnings set for ${TARGET}: ${WARNINGS}")
    endif()
endfunction()
