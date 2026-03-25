##
## Game main executable for portable
## sdl3Quake.exe
set( EXECUTABLE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/cd_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glimp_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/in_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/net_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL3_shared.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snddma_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sys_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vid_SDL3.c
    )

#Set egine main executable source
add_executable( sdl3Quake ${EXECUTABLE_SOURCES} ) #TODO: append on source on windows platforms 

#Set engine main definnitio
target_compile_definitions( sdl3Quake PRIVATE __ENGINE_EXE__ ) 

# Link openGL Libraries
target_link_libraries( sdl3Quake 
    PUBLIC 
        OpenGL::GL 
        SDL3::SDL3
    PRIVATE
        -Wl,--start-group
        qServer
        qCommon
        qClient 
        qShared
        -Wl,--end-group
    )

# set the precompiled header
target_precompile_headers( sdl3Quake 
	PUBLIC
        <ctype.h>
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
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL3_shared.h
	) 