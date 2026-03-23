##
## Capture The Flag - game logic source
## gameaxp.dll
set( GAME_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/g_ai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/g_chase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_cmds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_combat.c
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
    ${CMAKE_CURRENT_SOURCE_DIR}/g_turret.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/g_weapon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_actor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_actor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_berserk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_berserk.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss31.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss31.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss32.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_brain.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_brain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_chick.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_chick.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flipper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flipper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_float.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_float.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flyer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flyer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gladiator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gladiator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gunner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gunner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_hover.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_hover.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_infantry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_infantry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_insane.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_insane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_medic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_medic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_move.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_mutant.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_mutant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_parasite.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_parasite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_rider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_soldier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_soldier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_supertank.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_supertank.h
    ${CMAKE_CURRENT_SOURCE_DIR}/m_tank.c
    ${CMAKE_CURRENT_SOURCE_DIR}/m_tank.h
    ${CMAKE_CURRENT_SOURCE_DIR}/p_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_hud.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_trail.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_view.c
    ${CMAKE_CURRENT_SOURCE_DIR}/p_weapon.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.h
    )

set( GAME_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/game.def )

#set the library souce
add_library( game SHARED ${GAME_SOURCES} ) #TODO: append on source on windows platforms 

#set the game game specific definitions
target_compile_definitions( game PRIVATE __GAME_DLL__ ) 

#set the sound root dir as include
target_include_directories( game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( game 
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