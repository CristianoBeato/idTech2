##
## Game main executable for linux
## linQuake2.exe
set( EXECUTABLE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/block16.h
    ${CMAKE_CURRENT_SOURCE_DIR}/block8.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cd_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d_copy.s
    ${CMAKE_CURRENT_SOURCE_DIR}/d_ifacea.h
    ${CMAKE_CURRENT_SOURCE_DIR}/d_polysa.s
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_fxmesa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glob.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glob.h
    ${CMAKE_CURRENT_SOURCE_DIR}/in_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.AXP
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.i386
    ${CMAKE_CURRENT_SOURCE_DIR}/math.s
    ${CMAKE_CURRENT_SOURCE_DIR}/net_udp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shlinux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qasm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/r_aclipa.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_draw16.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_drawa.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_edgea.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_scana.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_spr8.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_surf8.s
    ${CMAKE_CURRENT_SOURCE_DIR}/r_varsa.s
    #${CMAKE_CURRENT_SOURCE_DIR}/rw_in_svgalib.c
    #${CMAKE_CURRENT_SOURCE_DIR}/rw_linux.h
    #${CMAKE_CURRENT_SOURCE_DIR}/rw_svgalib.c
    #${CMAKE_CURRENT_SOURCE_DIR}/rw_x11.c
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/snd_mixa.s
    ${CMAKE_CURRENT_SOURCE_DIR}/sys_dosa.s
    ${CMAKE_CURRENT_SOURCE_DIR}/sys_linux.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vid_menu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vid_so.c
    )

#Set egine main executable source
add_executable( linQuake2 ${EXECUTABLE_SOURCES} ) #TODO: append on source on windows platforms 

#Set engine main definnitio
target_compile_definitions( linQuake2 PRIVATE __ENGINE_EXE__ ) 

# Link openGL Libraries
target_link_libraries( linQuake2 
    PUBLIC 
        OpenGL::GL 
    PRIVATE
        qShared
        qServer
        qCommon
        qClient 
    )

# set the precompiled header
target_precompile_headers( linQuake2 
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
        #${CMAKE_CURRENT_SOURCE_DIR}/q_shared.h 
		#${CMAKE_CURRENT_SOURCE_DIR}/g_local.h
        #${CMAKE_CURRENT_SOURCE_DIR}/game.h
        #${CMAKE_CURRENT_SOURCE_DIR}/g_ctf.h
	) 