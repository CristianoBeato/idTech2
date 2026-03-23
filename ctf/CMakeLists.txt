##
## Capture The Flag - game logic source
## gameaxp.dll
set( CTF_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/g_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/g_chase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_cmds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_combat.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_ctf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_ctf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/g_func.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_items.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_local.h
    ${CMAKE_CURRENT_SOURCE_DIR}/g_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_misc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_monster.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_phys.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_save.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_spawn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_svcmds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_target.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_trigger.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_weapon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_move.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/p_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_hud.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_menu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_menu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/p_trail.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_view.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_weapon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.h
    )

set( CTF_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/ctf.def )

#set the library souce
add_library( gameaxp SHARED ${CTF_SOURCES} ) #TODO: append on source on windows platforms 

#set the gameaxp game specific definitions
target_compile_definitions( gameaxp PRIVATE __GAME_AXP_DLL__ ) 

#set the sound root dir as include
target_include_directories( gameaxp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( gameaxp 
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
        ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.h 
		${CMAKE_CURRENT_SOURCE_DIR}/g_local.h
        #${CMAKE_CURRENT_SOURCE_DIR}/game.h
        #${CMAKE_CURRENT_SOURCE_DIR}/g_ctf.h
	) 