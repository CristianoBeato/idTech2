##
## Server lib sources 
## qServer.so
set( SERVER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_ccmds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_ents.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_game.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_send.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_user.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_world.c
    )

#set the library souce
add_library( qServer STATIC ${SERVER_SOURCES} )

#set the library specific definitions
target_compile_definitions( qServer PRIVATE __SERVER_SO__ ) 

#set the shared dir as include
target_include_directories( qServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( qServer
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
	) 