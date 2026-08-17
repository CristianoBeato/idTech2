##
## Shared lib sources 
## qShared.so
set( SHARED_SOURCES 
    #${CMAKE_SOURCE_DIR}/game/m_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/q_Hunk.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/q_vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/q_vector.hpp
    )

set( GAME_SOURCE_DEF ${CMAKE_CURRENT_SOURCE_DIR}/game.def )

#set the library souce
add_library( qShared STATIC ${SHARED_SOURCES} ) #TODO: append on source on windows platforms 

#set the library specific definitions
target_compile_definitions( qShared PRIVATE __SHARED_SO__ ) 

#set the shared dir as include
target_include_directories( qShared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( qShared 
	PUBLIC
        <cstdio>
        <cstdlib>
        <cstdint>
        <cstdarg>
        <cstddef>
        <cassert>
        <cmath>
        <cstring>
        <ctime>
        <cctype>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/q_shared.hpp
	) 