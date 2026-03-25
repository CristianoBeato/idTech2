##
##
##
cmake_minimum_required( VERSION 3.13 )

##
## Project Name
##
project( idTech2 )

##
## Uses STD c++ 20
##
#set( CMAKE_CXX_STANDARD 20 )

##
## External Libraries
##

## SDL3
find_package( SDL3 REQUIRED CONFIG )
include_directories( SDL3::Headers )

## OpenGL
find_package( OpenGL REQUIRED )
include_directories( OpenGL::Headers )

##
## set the engine source root path
##
include_directories( ${CMAKE_SOURCE_DIR} )

##
## -fPIC
##
set( CMAKE_POSITION_INDEPENDENT_CODE ON )

##
## qShared
## shared libraries
add_subdirectory( shared )

##
## qCommon
## engine common libraries
add_subdirectory( qcommon )

##
## qClient
## client side common libraries
add_subdirectory( client )

##
## qServer
## shared libraries
add_subdirectory( server )

##
## ctf 
## Caputre The Flag game
add_subdirectory( ctf )

##
## game 
## Game logic 
add_subdirectory( game )



##
## ref_soft 
## Software renderer
#add_subdirectory( ref_soft )

##
## ref_gl 
## GL renderer
add_subdirectory( ref_gl )

##
## quake2 
## Engine main executable
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detectado Linux")
    add_subdirectory( linux  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Detectado Windows")
    add_subdirectory( win32 )
endif()

add_subdirectory( SDL3 )