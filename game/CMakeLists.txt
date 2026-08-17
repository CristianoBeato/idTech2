##
## single player game logic source
## game.dll
set( GAME_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/g_ai.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_chase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_cmds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_combat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_func.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_items.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_local.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_misc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_monster.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_phys.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_save.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_spawn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_svcmds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_target.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_trigger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_turret.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/g_weapon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_actor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_actor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_berserk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_berserk.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss31.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss31.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss32.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_boss3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_brain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_brain.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_chick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_chick.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flipper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flipper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_float.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_float.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flyer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_flyer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gladiator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gladiator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_gunner.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_hover.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_hover.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_infantry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_infantry.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_insane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_insane.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_medic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_medic.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_move.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_mutant.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_mutant.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_parasite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_parasite.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_player.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_rider.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_soldier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_soldier.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_supertank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_supertank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_tank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/m_tank.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/p_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/p_hud.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/p_trail.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/p_view.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/p_weapon.cpp
    )

set( GAME_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/game.def )

#set the library souce
add_library( game SHARED ${GAME_SOURCES} ) #TODO: append on source on windows platforms 

#set lib shared sources
target_link_libraries( game qShared )

#set the game specific definitions
target_compile_definitions( game PRIVATE __GAME_DLL__ ) 

#set the sound root dir as include
target_include_directories( game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

#set the precompiled header
target_precompile_headers( game 
	PUBLIC
        <cstdio>
        <cstdlib>
        <cstdint>
        <cstdarg>
        <cstddef>
        <cassert>
        <cmath>
        <cctype>
        <cstring>
        <ctime>
	PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/q_shared.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/g_local.hpp
	) 