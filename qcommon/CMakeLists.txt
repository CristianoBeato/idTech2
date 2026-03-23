##
## Engine common lib sources 
## qCommon.so
set( COMMON_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/cmd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cmodel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/crc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cvar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/files.c
    ${CMAKE_CURRENT_SOURCE_DIR}/md4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/net_chan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmove.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qcommon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qfiles.h
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
        <stdio.h>
        <stdlib.h>
        <stdint.h>
        <stdarg.h>
        <stddef.h>
        <stdbool.h>
        <assert.h>
        <math.h>
        <string.h>
        <time.h>	
    PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/q_shared.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qcommon.h
	) 