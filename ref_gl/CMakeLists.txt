##
## Capture The Flag - game logic source
## ref_gl.dll
set( RENDERER_GL_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/anorms.h
    ${CMAKE_CURRENT_SOURCE_DIR}/anormtab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_draw.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_light.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_local.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_model.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rmain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rmisc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rsurf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_warp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/warpsin.h
    )

set( RENDERER_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ref_gl.def )

#set the library souce
add_library( ref_gl SHARED ${RENDERER_GL_SOURCES} ) #TODO: append on source on windows platforms 

#set the openGL Renderer specific definitions
target_compile_definitions( ref_gl PRIVATE __RENDERER_GL_DLL__ ) 

#set the sound root dir as include
target_include_directories( ref_gl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# Link openGL Libraries
target_link_libraries( ref_gl PRIVATE qShared OpenGL::GL )

# set the precompiled header
target_precompile_headers( ref_gl 
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
		#${CMAKE_CURRENT_SOURCE_DIR}/g_local.h
        #${CMAKE_CURRENT_SOURCE_DIR}/game.h
        #${CMAKE_CURRENT_SOURCE_DIR}/g_ctf.h
	) 