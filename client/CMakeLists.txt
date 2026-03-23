##
## Engine common lib sources 
## qClient.so
set( CLIENT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/adivtab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/anorms.h
    ${CMAKE_CURRENT_SOURCE_DIR}/asm_i386.h
    ${CMAKE_CURRENT_SOURCE_DIR}/block16.h
    ${CMAKE_CURRENT_SOURCE_DIR}/block8.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cdaudio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_cin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_ents.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_fx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_input.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_inv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_newfx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_pred.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_scrn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_tent.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cl_view.c
    ${CMAKE_CURRENT_SOURCE_DIR}/client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/console.c
    ${CMAKE_CURRENT_SOURCE_DIR}/console.h
    ${CMAKE_CURRENT_SOURCE_DIR}/input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/keys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/keys.h
    ${CMAKE_CURRENT_SOURCE_DIR}/menu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qmenu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ref.h
    ${CMAKE_CURRENT_SOURCE_DIR}/screen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_loc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_mem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_mix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sound.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/x86.c
    )

#set the library souce
add_library( qClient STATIC ${CLIENT_SOURCES} )

#set the library specific definitions
target_compile_definitions( qClient PRIVATE __CLIENT_SO__ ) 

#set the shared dir as include
target_include_directories( qClient PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( qClient
	PUBLIC
        <stdio.h>
        <stdlib.h>
        <stdint.h>
        <stdarg.h>
        <stddef.h>
        <stdbool.h>
        <ctype.h>
        <assert.h>
        <math.h>
        <string.h>
        <time.h>	
    PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/q_shared.h
	) 