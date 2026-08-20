##
## Server lib sources 
## qServer.so
set( SERVER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/server.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_ccmds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_ents.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_send.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_user.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sv_world.cpp
    )

#set the library souce
add_library( qServer STATIC ${SERVER_SOURCES} )

#set the library specific definitions
target_compile_definitions( qServer PRIVATE __SERVER_SO__ ) 

#set the shared dir as include
target_include_directories( qServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled header
target_precompile_headers( qServer
	PUBLIC
        <cstdio>
        <cstdlib>
        <cstdint>
        <cstdarg>
        <cstddef>
        <cassert>
        <cctype>
        <cmath>
        <cstring>
        <ctime>	
    PRIVATE
        ${CMAKE_SOURCE_DIR}/shared/q_shared.hpp
	) 