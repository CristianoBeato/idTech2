##
## Capture The Flag - game logic source
## renderer_gl.dll
set( RENDERER_GL_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/anorms.h
    ${CMAKE_CURRENT_SOURCE_DIR}/anormtab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_draw.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_local.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rmain.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rmisc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_rsurf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_warp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/warpsin.h
    )

set( RENDERER_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/renderer_gl.def )

#set the library souce
add_library( renderer_gl SHARED ${RENDERER_GL_SOURCES} ) #TODO: append on source on windows platforms 

#set the openGL Renderer specific definitions
target_compile_definitions( renderer_gl PRIVATE __RENDERER_GL_DLL__ ) 

#set the sound root dir as include
target_include_directories( renderer_gl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# Link openGL Libraries
target_link_libraries( renderer_gl PRIVATE qShared OpenGL::GL )

# set the precompiled header
target_precompile_headers( renderer_gl 
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
        ${CMAKE_SOURCE_DIR}/shared/q_shared.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl_local.hpp
	) 