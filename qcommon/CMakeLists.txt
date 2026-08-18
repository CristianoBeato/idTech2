##
## Engine common lib sources 
## qCommon.so
set( COMMON_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/cmd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crc.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cvar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/files.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/md4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/net_chan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pmove.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qcommon.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qfiles.hpp
    )

#set the library souce
add_library( qCommon STATIC ${COMMON_SOURCES} )

#set the library specific definitions
target_compile_definitions( qCommon PRIVATE __COMMON_SO__ ) 

#set the shared dir as include
target_include_directories( qCommon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( qCommon
	PUBLIC
        <cstdio>
        <cstdlib>
        <cstdint>
        <cstdarg>
        <cstddef>
        <cassert>
        <cmath>
        <cstring>
        <ctime>
        <cctype>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/q_shared.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qcommon.hpp
	) 