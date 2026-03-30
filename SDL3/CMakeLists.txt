##
## Game main executable for portable
## sdl3Quake.exe
set( EXECUTABLE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/cd_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/con_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glimp_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/in_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/SDL3_shared.h
    ${CMAKE_CURRENT_SOURCE_DIR}/snddma_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sys_SDL3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vid_SDL3.c
    )

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detectado Linux")
    set( EXECUTABLE_NER_SOURCES
        ${CMAKE_SOURCE_DIR}/linux/net_udp.c
        )

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Detectado Windows")
    
    set( EXECUTABLE_NER_SOURCES 
        ${CMAKE_SOURCE_DIR}/win32/net_wins.c
        ${CMAKE_SOURCE_DIR}/win32/conproc.c
    )
endif()

#Set egine main executable source
add_executable( sdl3Quake ${EXECUTABLE_SOURCES} ${EXECUTABLE_NER_SOURCES} ) #TODO: append on source on windows platforms 

#Set engine main definnitio
target_compile_definitions( sdl3Quake PRIVATE __ENGINE_EXE__ ) 

#
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set( PLATFORM_LIBS )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set( PLATFORM_LIBS winmm ws2_32 )
endif()

# Link openGL Libraries
target_link_libraries( sdl3Quake 
    PUBLIC
        m 
        OpenGL::GL 
        SDL3::SDL3
        ${PLATFORM_LIBS}
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
        ${CMAKE_SOURCE_DIR}/qcommon/qcommon.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL3_shared.h
	) 